use std::env;
use std::net::{SocketAddr, TcpStream};
use std::time::Duration;

const MAX_PORT: u16 = 65535; // Maximum valid port number
const TIMEOUT: u64 = 1;      // Timeout in seconds for socket operations

// Function to scan a single port on a given IP address
fn scan_port(ip_addr: &str, port: u16) -> bool {
    let addr = format!("{}:{}", ip_addr, port);
    let socket_addr: SocketAddr = addr.parse().unwrap();
    
    // Set the timeout duration
    let timeout = Duration::new(TIMEOUT, 0);
    
    // Attempt to connect to the server
    match TcpStream::connect_timeout(&socket_addr, timeout) {
        Ok(_) => true,  // Port is open
        Err(_) => false, // Port is closed
    }
}

// Function to scan a range of ports
fn scan_ports(ip_addr: &str, start_port: u16, end_port: u16) {
    for port in start_port..=end_port {
        if scan_port(ip_addr, port) {
            println!("Port {} is open", port);
        } else {
            println!("Port {} is closed", port);
        }
    }
}

fn main() {
    // Ensure the correct number of command-line arguments
    let args: Vec<String> = env::args().collect();
    if args.len() != 4 {
        eprintln!("Usage: {} <IP> <start_port> <end_port>", args[0]);
        std::process::exit(1);
    }

    // Extract IP address and port range from command-line arguments
    let ip_addr = &args[1];
    let start_port: u16 = args[2].parse().expect("Invalid start port");
    let end_port: u16 = args[3].parse().expect("Invalid end port");

    // Validate port range to ensure it's within acceptable limits
    if start_port < 1 || end_port > MAX_PORT || start_port > end_port {
        eprintln!("Invalid port range");
        std::process::exit(1);
    }

    // Initiate the port scanning process
    scan_ports(ip_addr, start_port, end_port);
}