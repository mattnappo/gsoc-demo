pub mod acceptor;
pub mod proposer;

use crate::netutil;
use log::*;
use std::error::Error;
use std::net::{SocketAddrV4, TcpListener, TcpStream};
use std::str;
use std::sync::{Arc, Mutex};
use std::thread;

use serde::{Deserialize, Serialize};

type Key = String;
type Value = String;

/// The maximum size, in bytes, of an entire message on the network
pub const MAX_MSG_SIZE: usize = 512;
//pub const MAX_MSG_SIZE: usize = 8192;

#[derive(Debug, Serialize, Deserialize, Clone)]
pub enum Agent {
    Client,
    Proposer,
    Acceptor,
    Learner,
}

#[derive(Debug, Serialize, Deserialize, Clone)]
pub struct MessageHeader {
    pub from_addr: String,
    pub agent: Agent, // From agent
}

/// A message on the network between agents
#[derive(Debug, Serialize, Deserialize, Clone)]
pub enum MessageBody {
    /// Messages between Paxos nodes
    Prepare(u32),
    Promise(u32, Option<Entry<Key, Value>>),
    Accept(u32, Entry<Key, Value>),
    Accepted(u32, Entry<Key, Value>),
    Nack,
}

/// A message on the Paxos network
#[derive(Debug, Serialize, Deserialize, Clone)]
pub struct Message {
    pub header: MessageHeader,
    pub body: MessageBody,
}

/// The contents of a message which can be sent on the network
#[derive(Debug, Serialize, Deserialize, Clone)]
pub struct Entry<K, V>
where
    K: Sized,
    V: Sized,
{
    pub key: K,
    pub val: Option<V>,
}

/// A peer on the network
/// Each peer should be able to handle connections from all other kinds of peers
pub trait Peer: Clone {
    fn address(&self) -> SocketAddrV4;
    fn agent(&self) -> Agent;
    fn message(&self, body: MessageBody) -> Message {
        Message {
            header: MessageHeader {
                from_addr: self.address().to_string(),
                agent: self.agent(),
            },
            body,
        }
    }

    fn handle_client(&mut self, stream: TcpStream, message: &str) -> Result<(), Box<dyn Error>>;

    fn handle_proposer(
        &mut self,
        stream: TcpStream,
        message: Message,
    ) -> Result<(), Box<dyn Error>>;

    fn handle_acceptor(
        &mut self,
        stream: TcpStream,
        message: Message,
    ) -> Result<(), Box<dyn Error>>;

    fn handle_conn(&mut self, mut stream: TcpStream) -> Result<(), Box<dyn Error>> {
        let message = netutil::read_stream(&mut stream)?;

        if let Ok(message) = bincode::deserialize::<Message>(&message) {
            info!("{:?} received message {message:?}", self.address());
            match message.header.agent {
                Agent::Proposer => self.handle_proposer(stream, message)?,
                Agent::Acceptor => self.handle_acceptor(stream, message)?,
                _ => unreachable!(),
            }
        } else {
            let message = str::from_utf8(&message)?;
            info!("{:?} received message {message:?}", self.address());
            self.handle_client(stream, message)?;
        }
        Ok(())
    }

    /// Listen for connections from other peers
    fn listen(self) -> Result<(), Box<dyn Error>>;
}
