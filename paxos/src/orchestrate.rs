use log::*;
use std::env;
use std::error::Error;
use std::process::Command;

fn main() -> Result<(), Box<dyn Error>> {
    let args: Vec<String> = env::args().collect();
    if args.len() != 3 {
        panic!("usage: ./paxos258kv <base port> <num servers>");
    }
    env_logger::init();

    let base_port: u16 = args[1].parse()?;
    let num_acceptors: usize = args[2].parse()?;

    // Start acceptors/learners
    let acceptors = (0..num_acceptors).map(|i| {
        let port = (base_port + 1 + i as u16).to_string();
        let id = Command::new("./target/release/acceptor")
            .args([&port])
            .spawn()
            .unwrap()
            .id();
        info!("started leader-proposer on port {port:?} with pid {id:?}");
        format!("127.0.0.1:{port}")
    });

    // Start leader proposer
    let mut proposer_args = vec![base_port.to_string()];
    proposer_args.extend(acceptors);
    let proposer_pid = Command::new("./target/release/proposer")
        .args(proposer_args)
        .spawn()
        .unwrap()
        .id();

    info!("started leader-proposer on port {base_port:?} with pid {proposer_pid:?}");

    Ok(())
}
