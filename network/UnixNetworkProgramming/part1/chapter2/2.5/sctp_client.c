#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

#define PORT 8040
#define SERVER_IP "127.0.0.1"

int main()
{
    int sctp_socket;
    struct sockaddr_in server_addr;
    const char *message = "Привет, SCTP! 🎉";

    sctp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sctp_socket < 0)
    {
        perror("Ошибка создания SCTP сокета 😢");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sctp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Ошибка connect() 😤");
        close(sctp_socket);
        exit(EXIT_FAILURE);
    }

    /*
    int sctp_sendmsg(int sd, const void * msg, size_t len,
                    struct sockaddr *to, socklen_t tolen,
                    uint32_t ppid, uint32_t flags,
                    uint16_t stream_no, uint32_t timetolive,
                    uint32_t context);
    */
    if (sctp_sendmsg(sctp_socket, message, strlen(message), NULL, 0, 0, 0, 0, 0, 0) < 0)
    {
        perror("Ошибка sctp_sendmsg() 😵");
    }
    else
    {
        printf("Сообщение отправлено! ✅\n");
    }

    close(sctp_socket);
    return 0;
}