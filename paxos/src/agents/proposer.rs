use paxos_kv::protocol::{proposer::Proposer, Peer};
use std::env;
use std::net;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args = env::args().collect::<Vec<String>>();
    if args.len() < 2 {
        panic!("usage:\n\t./proposer <port> <acceptors>\n");
    }

    let acceptors = args[2..]
        .iter()
        .map(|x| x.parse().unwrap())
        .collect::<Vec<net::SocketAddrV4>>();

    env_logger::init();
    let proposer = Proposer::new("127.0.0.1", args[1].parse()?, acceptors);
    proposer.listen()?;

    Ok(())
}
