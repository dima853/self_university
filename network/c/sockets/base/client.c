#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    // 1. Создаём сокет
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket() failed");
        exit(1);
    }

    // 2. Настраиваем адрес сервера
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT)
    };
    
    // Преобразуем IP из строки в бинарный формат
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton() failed");
        exit(1);
    }

    // 3. Подключаемся к серверу
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect() failed");
        exit(1);
    }

    printf("Connected to server!\n");

    // 4. Отправляем сообщение
    char* message = "Hello from client!";
    send(sock, message, strlen(message), 0);

    // 5. Получаем ответ
    char buffer[1024] = {0};
    read(sock, buffer, sizeof(buffer));
    printf("Server says: %s\n", buffer);

    // 6. Закрываем соединение
    close(sock);
    return 0;
}