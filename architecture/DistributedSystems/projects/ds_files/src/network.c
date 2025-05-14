#include <stdio.h>       /* Standard input/output library for functions like printf, fprintf, etc. */
#include <stdlib.h>      /* Standard library for functions like malloc, free, exit, and other utilities. */
#include <string.h>      /* String manipulation library for functions like strcpy, strncpy, strlen, etc. */
#include <sys/socket.h>  /* Socket programming library for functions like socket, bind, listen, accept, etc. */
#include <sys/epoll.h>   /* Epoll library for asynchronous event notification. */
#include <netinet/in.h>  /* Internet address family for structures like sockaddr_in. */
#include <unistd.h>      /* POSIX operating system API for functions like close, read, write, etc. */
#include <arpa/inet.h>   /* Internet address conversion functions like inet_ntoa. */
#include <errno.h>       /* Error number definitions (e.g., EWOULDBLOCK, EINTR). */
#include <ctype.h>

#define PORT 8080          /* Define the port number on which the server will listen for incoming connections. */
#define MAX_PEERS 10       /* Define the maximum number of peers that can be tracked in the peer list. */
#define BUFFER_SIZE 1024    /* Define the size of the buffer used for reading and writing data to sockets. */
#define MAX_EVENTS 10       /* Define the maximum number of events that can be handled in a single epoll_wait call. */

uint32_t hex_to_int(char hex) {
    uint32_t val = 0;
    while (hex) {
        uint8_t byte = hex++;

        if ((byte >= '0' && byte <= '9') ||
            (byte >= 'a' && byte <= 'f') ||
            (byte >= 'A' && byte <= 'F')) {
            if (byte >= '0' && byte <= '9') byte = byte - '0';
            else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
            else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;

            val = (val << 4) | (byte & 0xF);
        } else {
            return 0; 
        }
    }
    return -1;
}

/*
  int high = hex_to_int(src[i+1]) Converts the first hexadecimal character (after %) to a numeric value. hex_to_int is an auxiliary function (not shown in the code) that must convert a hexadecimal character (for example, ‘A’, ‘3’, ‘f’) to its numeric representation (for example, 10, 3, 15).
  int low = hex_to_int(src[i+2]) Converts the second hexadecimal character (after %) to a numeric value.
  
  dest[decoded_idx++] = (high << 4)| low; 
  Example - ("%41")
  ---------------------------------------
  high = 4 (0000 0100)
  high << 4 = 0100 0000 (64)

  low = 1 (0000 0001)

  (high << 4) | low = 0100 0000 | 0000 0001 = 0100 0001 (65)
  ----------------------------------------------------------
*/
char* url_decode(const char* src, char* dest, size_t dest_size) {
    if (!src || !dest || dest_size == 0) return NULL;
    
    size_t decoded_idx = 0;
    size_t src_len = strlen(src);
    
    for (size_t i = 0; i < src_len && decoded_idx < dest_size - 1; i++) {
        if (src[i] == '%') {
            if (i + 2 >= src_len) return NULL;
            
            int high = hex_to_int(src[i+1]);
            int low = hex_to_int(src[i+2]);
            
            if (high == -1 || low == -1) return NULL;
            
            dest[decoded_idx++] = (high << 4) | low;
            i += 2;
        }
        else if (src[i] == '+') {
            dest[decoded_idx++] = ' ';
        }
        else {
            dest[decoded_idx++] = src[i];
        }
    }
    
    dest[decoded_idx] = '\0';
    return dest;
    
}

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
    int bytes_received;   
    char json_response[2048] = {0};
    char http_response[2048] = {0};/* Integer to store the number of bytes received from the client. */

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
    buffer[bytes_received] = '\0';
    
    // http parser
    /*
    char method[16] = {0}
    Declares a 16-byte method array for storing HTTP methods (GET, POST, etc.) and initializes it with zeros. 
    This is important to ensure that the array starts with an empty string.
    
    char uri[256] = {0} 
    Declares a 256 byte uri array for storing the URI (Uniform Resource Identifier) and initializes it with zeros.
    
    char *http_version = NULL
    Declares an http_version pointer to a string for storing the HTTP version (for example, “HTTP/1.1") and initializes it with NULL. 
    Using NULL is useful here, as it allows you to check if space has been allocated for http_version later.
    
    char *request_line_end =str str(buffer, "\r\n")
    Uses the strstr function to search for the first occurrence of newline characters (\r\n) in the buffer buffer. 
    The result (a pointer to the beginning of \r\t) is saved in request_line_end. \r\t usually marks the end of the query string.
    */
    char method[16] = {0};
    char uri[256] = {0};
    char *http_version = NULL;
    char *request_line_end = strstr(buffer, "\r\n");
    
    if (request_line_end == NULL) {
      const char* error_response = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\n\nInvalid request line";
      send(socket, error_response, strlen(error_response), 0);
      close(socket);
      return;
    }
    
        if (http_version == NULL) {
        const char* error_response = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\n\nInvalid HTTP version";
        send(socket, error_response, strlen(error_response), 0);
        close(socket);
        return;
    }
    // Parse the HTTP method, URI, and version
    sscanf(buffer, "%15s %255s %ms", method, uri, &http_version);
    
    printf("Method: %s, URI: %s\n", method, uri);
    printf("HTTP Version: %s\n", http_version);

    // Извлекаем заголовки
    char *header_start = request_line_end + 2; // Пропускаем \r\n
    char *header_end = strstr(header_start, "\r\n\r\n");

    char headers[2048] = {0};
    if (header_end != NULL) {
        int header_length = header_end - header_start;
        url_decode(headers, header_start, header_length);
        headers[header_length] = '\0';
        printf("Headers:\n%s\n", headers);
    }
    else{
        const char* error_response = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\n\nInvalid Headers";
        send(socket, error_response, strlen(error_response), 0);
        close(socket);
        return;
    }

    // Обрабатываем GET-запросы
    if (strcmp(method, "GET") == 0) {
        if (strcmp(uri, "/") == 0) { // Главная страница
             snprintf(json_response, sizeof(json_response),
                    "{\"message\": \"Welcome to my server!\", \"ip\": \"%s\", \"port\": %d}",
                    client_ip, client_port);
        } else if (strcmp(uri, "/hello") == 0) {
             snprintf(json_response, sizeof(json_response),
                    "{\"message\": \"Hello, world!\", \"client_ip\": \"%s\", \"method\": \"%s\"}",
                    client_ip, method);
        } else if (strcmp(uri, "/headers") == 0) {
              char encoded_headers[4096] = {0}; 
              url_decode(encoded_headers, headers, sizeof(encoded_headers));
             snprintf(json_response, sizeof(json_response),
                     "{\"headers\": \"%s\"}", encoded_headers); // Кодируем заголовки для корректной передачи в JSON
        }
        else {
            // 404 Not Found
            snprintf(http_response, sizeof(http_response),
                "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 9\n\nNot Found");
            send(socket, http_response, strlen(http_response), 0);
            close(socket);
            return; // Exit function to avoid sending the JSON response
        }

        // Формируем HTTP-ответ с JSON
        snprintf(http_response, sizeof(http_response),
                "HTTP/1.1 200 OK\nContent-Type: application/json\nContent-Length: %ld\n\n%s",
                strlen(json_response), json_response);
        send(socket, http_response, strlen(http_response), 0);

    } else {
        // Обработка других методов (POST, PUT, DELETE и т.д.)  (TODO)
        const char* error_response = "HTTP/1.1 501 Not Implemented\nContent-Type: text/plain\n\nNot Implemented";
        send(socket, error_response, strlen(error_response), 0);
    }

    free(http_version); //
 
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
 // Добавляем перед main()

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
    
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl: add server_fd");
        close(epoll_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    /*
     * Creates an epoll instance using the 'epoll_create1' function.
     *   - 0: Specifies that no flags are used.
     * The returned epoll file descriptor is stored in the 'epoll_fd' variable.
     * If epoll creation fails, an error message is printed using 'perror', and the program exits.
     */

      while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // Ожидаем события
        if (nfds == -1) {
            perror("epoll_wait");
            continue; // Продолжаем слушать, даже если epoll_wait вернул ошибку
        }

         for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_fd) {
                // 6. Новое входящее соединение
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                    perror("accept");
                    continue; // Продолжаем слушать, даже если accept не удался
                }

                // 7. Настраиваем событие для нового сокета
                event.events = EPOLLIN; // Интересуемся чтением данных
                event.data.fd = new_socket;

                // 8. Добавляем новый сокет в epoll
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                    perror("epoll_ctl: add new_socket");
                    close(new_socket);
                    continue;  // Продолжаем слушать, даже если добавление нового сокета не удалось
                }

                 printf("New connection accepted\n");

            } else {
                // 9. Обрабатываем существующее соединение (чтение данных)
                handle_connection(events[i].data.fd);  // events[i].data.fd - дескриптор сокета, готового к чтению
            }
         }
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
    close(epoll_fd);
    return 0;         /* Return 0 to indicate successful execution (This code will never be executed because the while loop is infinite.) */
}

/*
 * Defines the main function of the program.
 * This function creates a socket, sets socket options, configures the server address, binds the socket to the address, listens for incoming connections, and accepts and handles connections in an infinite loop.
 *
 * NOTE: This code is NOT using epoll() for handling incoming connections.  It is using blocking calls.  To use epoll() effectively, it should be used to monitor the *server_fd* and newly accepted sockets for readability.  When a new socket becomes readable (because a new connection is made to the server_fd) then accept() should be called.   Similarly, epoll should be used to monitor sockets for readability before calling read(), and for writability before calling send().
 */
