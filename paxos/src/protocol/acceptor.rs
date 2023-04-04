use crate::protocol::*;
use log::*;
use std::collections::HashMap;
use std::error::Error;
use std::io::Write;
//use std::fs::File;
//use std::io::{BufRead, BufReader, Write};
//use crate::{netutil, CustomError};
use std::net::{SocketAddrV4, TcpStream};

/// An acceptor is also a learner
#[derive(Debug, Clone)]
pub struct Acceptor {
    address: SocketAddrV4,
    gen_clock: u32,
    learned_values: HashMap<Key, Value>,
}

impl Acceptor {
    pub fn new(ip: &str, port: u16) -> Self {
        Self {
            address: format!("{ip}:{port}").parse().unwrap(),
            gen_clock: 0,
            learned_values: HashMap::new(),
        }
    }
}

impl Peer for Acceptor {
    fn address(&self) -> SocketAddrV4 {
        self.address
    }
    fn agent(&self) -> Agent {
        Agent::Acceptor
    }

    fn handle_client(&mut self, stream: TcpStream, message: &str) -> Result<(), Box<dyn Error>> {
        info!(
            "{:?} received message {message:?} from client",
            self.address
        );
        Ok(())
    }

    fn handle_proposer(
        &mut self,
        mut stream: TcpStream,
        message: Message,
    ) -> Result<(), Box<dyn Error>> {
        info!(
            "{:?} received message {message:?} from proposer",
            self.address
        );

        match message.body {
            MessageBody::Prepare(n) => {
                // Send the promises

                let message: Message;
                if n > self.gen_clock {
                    let prev = None;
                    message = self.message(MessageBody::Promise(n, prev));
                    self.gen_clock = n; // TODO: gen_clock++ ???
                } else {
                    message = self.message(MessageBody::Nack);
                }
                stream
                    .write_all(&bincode::serialize(&message).unwrap())
                    .unwrap();
                stream.flush().unwrap();
                info!("{:?} wrote {message:?}", self.address());

                // Read accept
                let buf = netutil::read_stream(&mut stream).unwrap();
                let message = bincode::deserialize::<Message>(&buf).unwrap();
                info!("got message {message:#?}");

                let (n, v) = match message.body {
                    MessageBody::Accept(n, v) => (n, v),
                    _ => unreachable!(),
                };

                // Learn the value or return learned value
                let accepted_value = match v.val {
                    // PUT REQUEST
                    Some(value) => {
                        // PUT REQUEST
                        // Learn the value
                        self.learned_values.insert(v.key.clone(), value.clone());
                        value
                    }

                    // GET REQUEST
                    None => match self.learned_values.get(&v.key) {
                        Some(inner) => inner.to_owned(),
                        None => String::new(),
                    },
                };

                // Write Accepted
                let new = Entry {
                    key: v.key,
                    val: Some(accepted_value),
                };
                let accepted_message = self.message(MessageBody::Accepted(n, new));
                stream
                    .write_all(&bincode::serialize(&accepted_message).unwrap())
                    .unwrap();
            }
            MessageBody::Accept(n, v) => {
                info!("got Accept({n}, {v:?})");
            }
            _ => unimplemented!(),
        }

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
