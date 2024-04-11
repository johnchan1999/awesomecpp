#include "Socket.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
Socket::Socket(int fd) : _fd(fd) {}
Socket::Socket() {
  _fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0) {
    perror("socket");
    return;
  }
}
Socket::~Socket() { close(_fd); }

int Socket::fd() { return _fd; }
