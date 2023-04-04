use paxos_kv::protocol::{acceptor::Acceptor, Peer};
use std::env;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args = env::args().collect::<Vec<String>>();
    if args.len() != 2 {
        panic!("usage:\n\t./acceptor <port>\n");
    }

    env_logger::init();
    let acceptor = Acceptor::new("127.0.0.1", args[1].parse()?);
    acceptor.listen()?;

    Ok(())
}
