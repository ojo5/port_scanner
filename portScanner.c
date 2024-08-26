#include <stdio.h>     // For input/output operations (printf, fprintf)
#include <stdlib.h>    // For memory allocation and process control (exit)
#include <string.h>    // For string manipulation (strlen, strcpy)
#include <unistd.h>    // For Unix standard functions (close)
#include <arpa/inet.h> // For internet operations (socket, connect, htons, inet_pton)
#include <sys/types.h> // For data types used in system calls
#include <sys/socket.h> // For socket operations (socket, setsockopt)

#define MAX_PORT 65535 // Maximum valid port number
#define TIMEOUT 1      // Timeout in seconds for socket operations

// Function to scan a single port on a given IP address
int scan_port(const char *ip_addr, int port) {
    int sock;
    struct sockaddr_in server_addr;
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;

    // Create a new socket for TCP connections
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1; // Indicate failure
    }

    // Set the timeout option for the socket
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("Failed to set socket timeout");
        close(sock);
        return -1; // Indicate failure
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_addr, &server_addr.sin_addr);

    // Attempt to connect to the server
    int connection_status = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Close the socket after the attempt
    close(sock);

    // Return connection status: 0 for open, non-zero for closed
    return connection_status;
}

// Function to scan a range of ports
void scan_ports(const char *ip_addr, int start_port, int end_port) {
    for (int port = start_port; port <= end_port; port++) {
        int status = scan_port(ip_addr, port);
        if (status == 0) {
            printf("Port %d is open\n", port);
        } else {
            printf("Port %d is closed\n", port);
        }
    }
}

int main(int argc, char *argv[]) {
    // Ensure the correct number of command-line arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <start_port> <end_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Extract IP address and port range from command-line arguments
    const char *ip_addr = argv[1];
    int start_port = atoi(argv[2]); // Convert start port argument to integer
    int end_port = atoi(argv[3]);   // Convert end port argument to integer

    // Validate port range to ensure it's within acceptable limits
    if (start_port < 1 || end_port > MAX_PORT || start_port > end_port) {
        fprintf(stderr, "Invalid port range\n");
        exit(EXIT_FAILURE);
    }

    // Initiate the port scanning process
    scan_ports(ip_addr, start_port, end_port);

    return 0; // Exit the program
}