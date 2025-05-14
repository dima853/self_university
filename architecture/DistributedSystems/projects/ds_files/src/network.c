#include <stdio.h>       /* Standard input/output library for functions like printf, fprintf, etc. */
#include <stdlib.h>      /* Standard library for functions like malloc, free, exit, and other utilities. */
#include <string.h>      /* String manipulation library for functions like strcpy, strncpy, strlen, etc. */
#include <sys/socket.h>  /* Socket programming library for functions like socket, bind, listen, accept, etc. */
#include <sys/epoll.h>   /* Epoll library for asynchronous event notification. */
#include <netinet/in.h>  /* Internet address family for structures like sockaddr_in. */
#include <unistd.h>      /* POSIX operating system API for functions like close, read, write, etc. */
#include <arpa/inet.h>   /* Internet address conversion functions like inet_ntoa. */
#include <errno.h>       /* Error number definitions (e.g., EWOULDBLOCK, EINTR). */

#define PORT 8080          /* Define the port number on which the server will listen for incoming connections. */
#define MAX_PEERS 10       /* Define the maximum number of peers that can be tracked in the peer list. */
#define BUFFER_SIZE 1024    /* Define the size of the buffer used for reading and writing data to sockets. */
#define MAX_EVENTS 10       /* Define the maximum number of events that can be handled in a single epoll_wait call. */

typedef struct {
    char ip[16];           /* Character array to store the IP address of a peer (16 bytes for IPv4 address). */
    int port;              /* Integer to store the port number of a peer. */
} Peer;

/*
 * Defines a structure named 'Peer' with two members:
 *   - ip: A character array to store the IP address (max 15 characters + null terminator).
 *   - port: An integer to store the port number.
 * This structure is used to represent a peer (a connected client) in the P2P network.
 */

Peer peer_list[MAX_PEERS]; /* Array of Peer structures to store the list of connected peers. */
int peer_count = 0;          /* Integer to keep track of the number of connected peers in the peer_list. */

/*
 * Declares a global array 'peer_list' of type 'Peer' to store the information of up to MAX_PEERS peers.
 * 'peer_count' is a global integer variable used to keep track of the number of peers currently in the list.
 */

void add_peer(const char* ip, int port) {
    if (peer_count < MAX_PEERS) {  /* Check if the peer_list is not full. */
        strncpy(peer_list[peer_count].ip, ip, 16); /* Copy the IP address to the peer_list. */
        peer_list[peer_count].port = port; /* Assign the port number to the peer_list. */
        peer_count++; /* Increment the peer_count. */
        printf("Added new peer: %s:%d\n", ip, port); /* Print a message to the console indicating that a new peer has been added. */
    }
}

/*
 * Defines a function 'add_peer' that takes the IP address (as a string) and port number of a peer as input.
 * It adds the peer to the 'peer_list' if the list is not full (peer_count < MAX_PEERS).
 * 'strncpy' is used to copy the IP address to avoid buffer overflows.
 * The 'peer_count' variable is incremented after adding a new peer.
 * A message is printed to the console to indicate that a new peer has been added.
 */

int start_server() {
    int server_fd, new_socket;                /* Declare variables for the server socket file descriptor and new socket file descriptor. */
    struct sockaddr_in address;                 /* Declare a structure for the server address. */
    int opt = 1;                                 /* Option value for setsockopt to enable reuse of address and port. */
    int addrlen = sizeof(address);              /* Length of the address structure. */

    // Create a socket.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { /* Create a socket for incoming connections. */
        perror("socket failed"); /* Print an error message if socket creation fails. */
        exit(EXIT_FAILURE); /* Exit the program with a failure status. */
    }

    /*
     * Creates a socket file descriptor using the 'socket' function.
     *   - AF_INET: Specifies the IPv4 address family.
     *   - SOCK_STREAM: Specifies a reliable, connection-oriented byte stream (TCP).
     *   - 0: Uses the default protocol for the specified address family and socket type.
     * If socket creation fails, an error message is printed using 'perror', and the program exits.
     */

    // Set socket options to reuse address and port.
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) { /* Set socket options to allow reuse of the address. */
        perror("setsockopt");  /* Print an error message if setting socket options fails. */
        close(server_fd);   /* Close the server socket if setting options fails. */
        exit(EXIT_FAILURE);  /* Exit the program with a failure status. */
    }

    /*
     * Sets socket options using the 'setsockopt' function to allow reuse of the address and port.
     * This is useful when the server is restarted quickly, as it allows the new server to bind to the same address and port.
     *   - SOL_SOCKET: Specifies the socket level.
     *   - SO_REUSEADDR: Allows reuse of the address.
     *   - opt: Option value (1 to enable reuse).
     * If setting socket options fails, an error message is printed using 'perror', the server socket is closed, and the program exits.
     */

    // Configure the server address.
    address.sin_family = AF_INET;                /* Set the address family to IPv4. */
    address.sin_addr.s_addr = INADDR_ANY;         /* Allow connections on any available network interface. */
    address.sin_port = htons(PORT);               /* Set the port number. htons converts the port number from host byte order to network byte order. */

    /*
     * Populates the 'address' structure with the server's address information.
     *   - sin_family: Set to AF_INET to specify the IPv4 address family.
     *   - sin_addr.s_addr: Set to INADDR_ANY to allow connections on any available network interface.
     *   - sin_port: Set to the port number specified by the 'PORT' macro. 'htons' is used to convert the port number from host byte order to network byte order.
     */

    // Bind the socket to the specified address and port.
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { /* Bind the socket to the specified address and port. */
        perror("bind failed"); /* Print an error message if binding fails. */
        close(server_fd); /* Close the server socket if binding fails. */
        exit(EXIT_FAILURE);  /* Exit the program with a failure status. */
    }

    /*
     * Binds the socket to the specified address and port using the 'bind' function.
     *   - server_fd: Socket file descriptor.
     *   - (struct sockaddr *)&address: Pointer to the address structure.
     *   - sizeof(address): Size of the address structure.
     * If binding fails, an error message is printed using 'perror', the server socket is closed, and the program exits.
     */

    // Listen for incoming connections.
    if (listen(server_fd, 3) < 0) { /* Listen for incoming connections on the socket. */
        perror("listen"); /* Print an error message if listening fails. */
        close(server_fd); /* Close the server socket if listening fails. */
        exit(EXIT_FAILURE);  /* Exit the program with a failure status. */
    }

    /*
     * Listens for incoming connections on the socket using the 'listen' function.
     *   - server_fd: Socket file descriptor.
     *   - 3: Specifies the maximum length of the queue of pending connections.
     * If listening fails, an error message is printed using 'perror', the server socket is closed, and the program exits.
     */

    printf("Server listening on port %d\n", PORT); /* Print a message to the console indicating that the server is listening. */
    return server_fd; /* Return the server socket file descriptor. */
}

/*
 * Defines a function 'start_server' that creates a socket, sets socket options, configures the server address, binds the socket to the address, and listens for incoming connections.
 * If any of these operations fail, an error message is printed to the console, the server socket is closed, and the program exits.
 * The function returns the server socket file descriptor if successful.
 */

void handle_connection(int socket) {
    char buffer[BUFFER_SIZE] = {0}; /* Character array to store the data received from the client. */
    int bytes_received;                /* Integer to store the number of bytes received from the client. */

    // Get client IP address
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    getpeername(socket, (struct sockaddr*)&client_address, &client_len);
    char* client_ip = inet_ntoa(client_address.sin_addr);
    int client_port = ntohs(client_address.sin_port);

    printf("New connection from %s:%d\n", client_ip, client_port);

    // Read data from the socket
    bytes_received = read(socket, buffer, BUFFER_SIZE - 1);  /* Read data from the client socket into the buffer. */

    if (bytes_received > 0) {  /* If bytes were received from the client */
        buffer[bytes_received] = '\0'; /* Null-terminate the received data. */
        printf("Received: %s\n", buffer); /* Print the data received from the client to the console. */
    } else if (bytes_received == 0) { /* If client disconnected */
        printf("Client disconnected\n"); /* Print a message to the console indicating that the client has disconnected. */
    } else {
        perror("recv failed"); /* If an error occurred during data reception */
    }

    const char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello from the server!";
    send(socket, response, strlen(response), 0);

    close(socket); /* Close the client socket. */
    printf("Connection closed\n");/* Print a message to the console indicating that the connection has been closed. */
}

/*
 * Defines a function 'handle_connection' that takes a socket file descriptor as input.
 * This function is responsible for handling the communication with a connected client.
 * It reads data from the socket, prints it to the console, sends a response back to the client, and closes the socket.
 * The function uses a buffer to store the data received from the client.
 * 'read' is used to receive data from the client.
 * If data is received, it's printed to the console.
 * If the client disconnects, a message is printed to the console.
 * If an error occurs during data reception, an error message is printed using 'perror'.
 * 'close' is used to close the socket after the communication is complete.
 */

int main() {
    struct sockaddr_in address;                  /* Declare a structure for the server address. */
    struct epoll_event event, events[MAX_EVENTS];  /* Declare structures for epoll events. */
    int server_fd, epoll_fd, new_socket;           /* Declare variables for socket file descriptors. */
    int addrlen = sizeof(address);               /* Length of the address structure. */

    server_fd = start_server();  /* Call the start_server function to create and initialize the server socket. */
    printf("Server is running and waiting for connections...\n"); /* Print a message to the console indicating that the server is running. */

    /*
     * Calls the 'start_server' function to create and initialize the server socket.
     * The returned server socket file descriptor is stored in the 'server_fd' variable.
     * A message is printed to the console indicating that the server is running and waiting for connections.
     */

    epoll_fd = epoll_create1(0);  /* Create an epoll instance. */
    if (epoll_fd == -1) { /* If creating the epoll instance fails */
        perror("epoll_create1"); /* Print an error message. */
        exit(EXIT_FAILURE);  /* Exit the program with a failure status. */
    }

    /*
     * Creates an epoll instance using the 'epoll_create1' function.
     *   - 0: Specifies that no flags are used.
     * The returned epoll file descriptor is stored in the 'epoll_fd' variable.
     * If epoll creation fails, an error message is printed using 'perror', and the program exits.
     */

    while (1) { /* Infinite loop to continuously accept incoming connections. */
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) { /* Accept a new incoming connection. */
            perror("accept");  /* Print an error message if accepting the connection fails. */
            continue;   /* Continue to the next iteration of the loop. */
        }

        handle_connection(new_socket);  /* Call the handle_connection function to handle the connection. */
    }

    /*
     * Enters an infinite loop to continuously accept incoming connections.
     * Inside the loop:
     *   - 'accept' is called to accept a new incoming connection on the server socket.
     *   - If 'accept' fails, an error message is printed using 'perror', and the loop continues to the next iteration.
     *   - If 'accept' is successful, the 'handle_connection' function is called to handle the connection.
     *
     * NOTE: This code is NOT using epoll(). It needs to be modified to use epoll for asynchronous connection handling.
     */

    close(server_fd); /* This code will never be executed because the while loop is infinite. */
    return 0;         /* Return 0 to indicate successful execution (This code will never be executed because the while loop is infinite.) */
}

/*
 * Defines the main function of the program.
 * This function creates a socket, sets socket options, configures the server address, binds the socket to the address, listens for incoming connections, and accepts and handles connections in an infinite loop.
 *
 * NOTE: This code is NOT using epoll() for handling incoming connections.  It is using blocking calls.  To use epoll() effectively, it should be used to monitor the *server_fd* and newly accepted sockets for readability.  When a new socket becomes readable (because a new connection is made to the server_fd) then accept() should be called.   Similarly, epoll should be used to monitor sockets for readability before calling read(), and for writability before calling send().
 */