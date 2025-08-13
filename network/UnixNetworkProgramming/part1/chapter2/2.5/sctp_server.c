#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

#define PORT 8040
#define BUFFER_SIZE 1024

int main()
{
    int sctp_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sctp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sctp_socket < 0)
    {
        perror("Ошибка создания SCTP сокета 😢");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(sctp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Ошибка bind() 😡");
        close(sctp_socket);
        exit(EXIT_FAILURE);
    }

    listen(sctp_socket, 5);
    printf("Сервер SCTP запущен на порту %d... 🚀\n", PORT);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(sctp_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket < 0)
    {
        perror("Ошибка accept() 😱");
        close(sctp_socket);
        exit(EXIT_FAILURE);
    }

    /*
    int sctp_recvmsg(int sd, void * msg, size_t len,
                    struct sockaddr * from, socklen_t * fromlen,
                    struct sctp_sndrcvinfo * sinfo, int * msg_flags);
    */
    ssize_t bytes_read = sctp_recvmsg(client_socket, buffer, BUFFER_SIZE, NULL, 0, NULL, NULL);
    if (bytes_read < 0)
    {
        perror("Ошибка sctp_recvmsg() 😵");
    }
    else
    {
        buffer[bytes_read] = '\0';
        printf("Получено сообщение: %s\n", buffer);
    }

    close(client_socket);
    close(sctp_socket);
    return 0;
}