#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd == -1) {
    perror("Couldn't create socket");
    return 1;
  }

  struct sockaddr_in adress;

  adress.sin_family = AF_INET;
  adress.sin_addr.s_addr = INADDR_ANY;
  adress.sin_port = htons(8080);

  if (bind(server_fd, (struct sockaddr *)&adress, sizeof(adress)) == -1) {
    perror("Couldn't bind");
    return 1;
  }

  if (listen(server_fd, 64) == -1) {
    perror("Couldn't listen");
    return 1;
  }

  int closed;

  while (1) {
    printf("Waiting for a connection...\n");

    socklen_t size = sizeof(adress);
    int accepted_socket_fd =
        accept(server_fd, (struct sockaddr *)&adress, &size);

    if (accepted_socket_fd == -1) {
      perror("Couldn't accept");
      if (errno == EMFILE) {
        break;
      }
      if (errno == ENFILE) {
        sleep(10);
      }
      continue;
    }

    char recv_buffer[1024];
    memset(recv_buffer, 'X', sizeof(recv_buffer));
    ssize_t received = recv(accepted_socket_fd, recv_buffer, 1024, 0);

    if (received == -1) {
      perror("Couldn't receive");
      close(accepted_socket_fd);
      continue;
    }

    printf("%.*s", (int)received, recv_buffer);

    char send_buffer[1024] = "";
    char response_body[512] = "Hello";
    snprintf(send_buffer, sizeof(send_buffer),
             "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\n\r\n%s",
             strlen(response_body), response_body);

    ssize_t sent =
        send(accepted_socket_fd, send_buffer, strlen(send_buffer), 0);

    if (sent == -1) {
      perror("Couldn't send");
      close(accepted_socket_fd);
      continue;
    }

    closed = close(accepted_socket_fd);
    if (closed == -1) {
      perror("Couldn't close accepted socket");
      return 1;
    }
  }

  closed = close(server_fd);
  if (closed == -1) {
    perror("Couldn't close server");
    return 1;
  }

  return 0;
}