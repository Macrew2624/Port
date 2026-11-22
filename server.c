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
  // adress.sin_addr = INADDR_ANY;

  int idk = bind(server_fd, INADDR_ANY, sizeof(INADDR_ANY));

  return 0;
}