/*
 * proxy.c - A Simple Sequential Web proxy
 */

 #include "csapp.h"
 #include <strings.h>
 #include <pthread.h> //Для туннелирования трафика
 
 #define MAX_HOSTNAME 256  // Максимальная длина имени хоста
 
 //prints out error message
 void myerror();
 
 // Функция для туннелирования трафика между двумя сокетами (нужна многопоточность)
 void *tunnel_traffic(void *arg);
 
 int main(int argc, char **argv) {
   char buffer[MAXLINE];
   int clientfd;
   int serverfd;
   struct sockaddr_in clientaddr;
   int clientlen;
   int n;
   rio_t rio;
   int port;
   int sockfd;
 
   // check arguments
 
   if (argc != 2) {
     fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
     exit(0);
   }
 
   port = atoi(argv[1]);
 
   //open connection on given port, bind, listen
 
   if ((sockfd = Open_listenfd(port)) < 0)
     myerror();
 
   //infinite server loop
 
   while (1) {
 
     // accept
 
     clientlen = sizeof(clientaddr);
 
     if ((clientfd = Accept(sockfd, (SA *)&clientaddr, &clientlen)) < 0)
       myerror();
 
     //display GET request
 
     n = recv(clientfd, buffer, MAXLINE - 1, 0); // recv может вернуть ошибку
     if (n < 0) {
       perror("recv");
       close(clientfd);
       continue;  // Переходим к следующему клиенту
     }
 
     buffer[n] = '\0'; // Важно завершить строку нулем
 
     printf("\nThe request in buffer is\n%s\n", buffer);
 
     // Проверяем, является ли запрос CONNECT запросом.
     if (strncmp(buffer, "CONNECT", 7) == 0) {
       //Обрабатываем CONNECT запрос
       char hostname[MAX_HOSTNAME];
       int remote_port;
       sscanf(buffer, "CONNECT %[^:]:%d", hostname, &remote_port);
 
       printf("CONNECT request to %s:%d\n", hostname, remote_port);
 
       // 1. Подключаемся к удаленному серверу
       serverfd = open_clientfd(hostname, remote_port);
       if (serverfd < 0) {
         char response[] = "HTTP/1.1 502 Bad Gateway\r\n\r\n";
         Rio_writen(clientfd, response, strlen(response));
         close(clientfd);
         continue;
       }
 
       // 2. Отправляем ответ 200 Connection Established клиенту
       char response[] = "HTTP/1.1 200 Connection Established\r\n\r\n";
       Rio_writen(clientfd, response, strlen(response));
         // Создаем структуру для передачи аргументов в поток
       int *fds = malloc(2 * sizeof(int));
       fds[0] = clientfd;
       fds[1] = serverfd;
 
       pthread_t thread;
       if (pthread_create(&thread, NULL, tunnel_traffic, (void *)fds) != 0) {
           perror("pthread_create");
           close(clientfd);
           close(serverfd);
           free(fds);
           continue;
       }
       pthread_detach(thread); //Отсоединяем поток, чтобы ресурсы освободились после завершения
         continue;
     }
 
     ///////////////////begin parse host request/////////////////////
 
     char method[100];
     char filename[500];
     char rest[1000];
 
     sscanf(buffer, "%s %s %s", method, filename, rest);
 
     printf("The FILENAME is initially %s\n", filename);
 
     char *host = strstr(filename, "www");
     if (!host) {  // Проверка на NULL
       fprintf(stderr, "Error: 'www' not found in filename\n");
       close(clientfd);
       continue;
     }
 
     char temp_host[500];  // Используем буфер фиксированного размера
     char temp_host2[500];
 
     strncpy(temp_host, host, sizeof(temp_host) - 1);  // strncpy безопаснее
     temp_host[sizeof(temp_host) - 1] = '\0';         // Всегда завершаем строку
     strncpy(temp_host2, host, sizeof(temp_host2) - 1);
     temp_host2[sizeof(temp_host2) - 1] = '\0';
 
     printf("The host string is initially %s\n", host);
 
     char *f = index(host, ':');
     if (f == NULL) {
       host = (char *)strtok(host, "/");
     } else {
       host = (char *)strtok(host, ":");
     }
 
     ///////////////end parse host request//////////////////////
 
     //open port 80 unless otherwise specified and connect to server at given host
 
     char *tmp = strstr(temp_host, ":"); // Use temp_host here
     int remote_port = 80;
 
     if (tmp != NULL) {
       remote_port = atoi(tmp + 1); // +1 to skip ':'
     }
       serverfd = open_clientfd(host, remote_port); // Используйте hostname
     if(serverfd < 0){
         myerror();
         close(clientfd);
         continue;
 
     }
 
     //send GET request to server
     Rio_writen(serverfd, "GET ", strlen("GET "));
     Rio_writen(serverfd, filename, strlen(filename));
     Rio_writen(serverfd, " HTTP/1.0\r\n\r\n", strlen(" HTTP/1.0\r\n\r\n"));
 
     //receive reply
 
     int response_len = 0;
 
     while ((n = rio_readn(serverfd, buffer, MAXLINE)) > 0) {
       response_len += n;
       Rio_writen(clientfd, buffer, n);
         bzero(buffer, MAXLINE);  // Очищаем буфер
     }
 
     close(clientfd);
     close(serverfd);
   }
 
   return 0;
 }
 
 //prints error message and quits
 void myerror() {
   perror("server");
   exit(1);
 }
 
 // Функция для туннелирования трафика (нужна многопоточность)
 void *tunnel_traffic(void *arg) {
   int clientfd = ((int*)arg)[0];
   int serverfd = ((int*)arg)[1];
   free(arg); // Освобождаем память, выделенную в основном потоке
   char buffer[MAXLINE];
   int n;
 
   while (1) {
     n = rio_readn(clientfd, buffer, MAXLINE);
     if (n <= 0) break;  // Ошибка или закрытие соединения
 
     Rio_writen(serverfd, buffer, n);
   }
 
   close(clientfd);
   close(serverfd);
   return NULL;
 }