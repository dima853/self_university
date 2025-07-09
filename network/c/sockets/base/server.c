#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    // 1. Создаём сокет
    int server_fd = socket(AF_INET, SOCK_STREAM, 0) // ipv4 | tcp | default protocol

    // 2. Настраиваем адрес сервера
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    }

    // 3. Привязываем сокет к адресу
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(1);
    }

    // 4. Слушаем входящие подключения (макс. 5 в очереди)
    if (listen(server_fd, 5) < 0) {
        perror("listen() failed");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // 5. Принимаем подключение
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_socket < 0) {
        perror("accept() failed");
        exit(1);
    }

    printf("Client connected!\n");
                                          
    // 6. Читаем данные от клиента 
    char buffer[1024] = {0};
    read(client_socket, buffer, sizeof(buffer));
    printf("Client says: %s\n", buffer);

    // 7. Отправляем ответ
    char* response = "Hello from server!";
    send(client_socket, response, strlen(response), 0);

    // 8. Закрываем соединение
    close(client_socket);
    close(server_fd);
    return 0;
}