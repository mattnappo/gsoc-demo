use crate::protocol::*;
use crate::{netutil, CustomError};
use log::*;
use std::collections::HashMap;
use std::error::Error;
use std::fs::File;
use std::io::{BufRead, BufReader, Write};
use std::net::{SocketAddrV4, TcpStream};
use std::sync::Arc;
use std::sync::Mutex;
use std::thread;

#[derive(Debug, Clone)]
pub struct Proposer {
    address: SocketAddrV4,
    gen_clock: u32,
    acceptors: Vec<SocketAddrV4>,
    learned_values: HashMap<Key, Value>,
}

impl Proposer {
    pub fn new(ip: &str, port: u16, acceptors: Vec<SocketAddrV4>) -> Self {
        Self {
            address: format!("{ip}:{port}").parse().unwrap(),
            gen_clock: 0,
            acceptors,
            learned_values: HashMap::new(),
        }
    }

    /// Return all the acceptors in the network
    pub fn find_quorum(&self) -> Result<Vec<SocketAddrV4>, Box<dyn Error>> {
        let alive_acceptors = self
            .acceptors
            .clone()
            .into_iter()
            .filter(|a| netutil::is_alive(&a))
            .collect::<Vec<SocketAddrV4>>();
        if alive_acceptors.len() <= 1 {
            Err(Box::new(CustomError::new("cannot find quorum")))
        } else {
            Ok(alive_acceptors)
        }
    }

    fn begin_paxos(
        &mut self,
        entry: Entry<Key, Value>,
    ) -> Result<Entry<Key, Value>, Box<dyn Error>> {
        // Find quorum of acceptors
        let quorum = self.find_quorum()?;

        // Construct the prepare message
        self.gen_clock += 1;
        let prepare_message = self.message(MessageBody::Prepare(self.gen_clock));
        let prepare_message = bincode::serialize(&prepare_message).unwrap();
        let prepare_message = Arc::new(Mutex::new(prepare_message));

        // (1a) Send Prepare(n) to a quorum
        //for acceptor in quorum.into_iter() {
        let handles = quorum.into_iter().map(|acceptor| {
            let message = prepare_message.clone();
            thread::spawn(move || match TcpStream::connect(acceptor) {
                Ok(mut stream) => {
                    let locked = message.lock().unwrap();
                    stream.write_all(&locked).unwrap();
                    stream.flush().unwrap();
                    info!("wrote Prepare to {acceptor:?}");
                    Some(stream)
                }
                Err(e) => {
                    error!("could not connect: {e:?}");
                    None
                }
            })
        });
        let streams = handles
            .map(|h| h.join().unwrap())
            .filter(|x| x.is_some())
            .flatten()
            .collect::<Vec<TcpStream>>();

        // (1b) Get Promises from each of the Prepares
        let handles = streams.into_iter().map(|mut acceptor| {
            thread::spawn(move || {
                let message = netutil::read_stream(&mut acceptor).unwrap();
                let promise = bincode::deserialize::<Message>(&message).unwrap();
                info!("received Promise {promise:#?}");
                (acceptor, promise)
            })
        });
        let (streams, promises): (Vec<TcpStream>, Vec<Message>) =
            handles.map(|h| h.join().unwrap()).unzip();
        if streams.len() != promises.len() {
            return Err(Box::new(CustomError::new(
                "failed to receive Promises from quorum",
            )));
        }

        /* -- begin 2a -- */

        // Compute the accepted value
        let mut promises = promises
            .into_iter()
            .map(|p| match p.body {
                MessageBody::Promise(n, v) => (n, v),
                _ => unimplemented!(),
            })
            .collect::<Vec<(u32, Option<Entry<Key, Value>>)>>();
        promises.sort_by(|a, b| a.0.partial_cmp(&b.0).unwrap());

        // If no value from network, then proposer sets value
        let accept_message: Message;
        if promises.iter().all(|p| p.1.is_none()) {
            accept_message = self.message(MessageBody::Accept(self.gen_clock, entry));
        } else {
            // Else, pick highest value from Acceptors
            let value = promises.into_iter().find(|p| p.1.is_some()).unwrap();
            accept_message = self.message(MessageBody::Accept(value.0, value.1.unwrap()));
        }

        // (2a) Send Accepts to the quorum
        for mut acceptor in streams.iter() {
            acceptor.write(&bincode::serialize(&accept_message)?)?;
            info!("wrote Accept {accept_message:#?}");
        }

        // (2b) Wait for Accepted message
        let mut values = streams
            .into_iter()
            .map(|mut acceptor| {
                let message = netutil::read_stream(&mut acceptor).unwrap();
                let message = bincode::deserialize::<Message>(&message).unwrap();
                info!("read Accepted {message:#?}");
                match message.body {
                    MessageBody::Accepted(n, v) => (n, v),
                    _ => unreachable!(),
                }
            })
            .collect::<Vec<(u32, Entry<Key, Value>)>>();

        // Compute the consensus value
        values = values.into_iter().filter(|x| x.1.val.is_some()).collect();
        values.sort_by(|a, b| a.0.partial_cmp(&b.0).unwrap());
        let accepted_value = values[0].1.clone();

        // Learn the value
        self.learned_values.insert(
            accepted_value.key.clone(),
            accepted_value.val.clone().unwrap(),
        );

        Ok(accepted_value)
    }
}

impl Peer for Proposer {
    fn address(&self) -> SocketAddrV4 {
        self.address
    }
    fn agent(&self) -> Agent {
        Agent::Proposer
    }

    fn handle_client(
        &mut self,
        mut stream: TcpStream,
        message: &str,
    ) -> Result<(), Box<dyn Error>> {
        // Parse the message
        let message = message.split_whitespace().collect::<Vec<&str>>();
        if message.len() < 2 {
            return Err(Box::new(CustomError::new("bad client request")));
        }

        // Handle the message
        match message[0] {
            "get" => {
                let val = self.begin_paxos(Entry {
                    key: message[1].to_string(),
                    val: None,
                });
                let get_val = val.unwrap().val.unwrap();
                stream.write_all(get_val.as_bytes()).unwrap();
            }
            "put" => {
                let _ = self
                    .begin_paxos(Entry {
                        key: message[1].to_string(),
                        val: Some(message[2..].join("\n")),
                    })
                    .unwrap();
                info!("");
            }
            _ => return Err(Box::new(CustomError::new("bad client request"))),
        }

        Ok(())
    }

    fn handle_proposer(
        &mut self,
        stream: TcpStream,
        message: Message,
    ) -> Result<(), Box<dyn Error>> {
        info!(
            "{:?} received message {message:?} from proposer {stream:?}",
            self.address
        );
        Ok(())
    }

    fn handle_acceptor(
        &mut self,
        stream: TcpStream,
        message: Message,
    ) -> Result<(), Box<dyn Error>> {
        info!(
            "{:?} received message {message:?} from acceptor {stream:?}",
            self.address
        );
        Ok(())
    }

    /// Listen for connections from other peers
    fn listen(self) -> Result<(), Box<dyn Error>> {
        let listener = TcpListener::bind(self.address())?;
        info!(
            "started {:?}: listening on {:?}",
            self.agent(),
            self.address()
        );

        // Listen for connections
        let shared = Arc::new(Mutex::new(self));

        for stream in listener.incoming() {
            let stream = stream?;
            let thread_shared = Arc::clone(&shared);
            thread::spawn(move || {
                let mut locked = thread_shared.lock().unwrap();
                info!("got connection from {stream:?}");
                locked.handle_conn(stream).unwrap();
            });
        }
        Ok(())
    }
}
