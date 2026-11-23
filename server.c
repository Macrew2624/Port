#include <netinet/in.h>
#include <stdio.h>
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

  socklen_t *size = sizeof(adress);
  int accepted_socket_fd = accept(server_fd, (struct sockaddr *)&adress, size);

  if (accepted_socket_fd == -1) {
    perror("Couldn't accept");
    return 1;
  }

  return 0;
}