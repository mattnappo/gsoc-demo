use crate::protocol::MAX_MSG_SIZE;
use std::io;
use std::io::Read;
use std::net::{SocketAddrV4, TcpStream};

pub fn read_stream(stream: &mut TcpStream) -> io::Result<Vec<u8>> {
    let mut buffer = [0; MAX_MSG_SIZE];
    let bytes_read = stream.read(&mut buffer)?;
    Ok(buffer[..bytes_read].to_vec())
}

/// Ping a peer to check if it is alive
pub fn is_alive(peer: &SocketAddrV4) -> bool {
    true // TODO
}
