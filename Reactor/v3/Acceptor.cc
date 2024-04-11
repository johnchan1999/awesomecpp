#include "Acceptor.h"
#include <sys/socket.h>

Acceptor::Acceptor(const string ip, unsigned short port)
    : _socket(), _InetAdress(ip, port) {}

Acceptor::~Acceptor() {}

void Acceptor::ready() {
  setReuseAddr();
  setReusePort();
  bind();
  listen();
}

void Acceptor::setReuseAddr() {
  int on = 1;

  int ret = setsockopt(_socket.fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  if (ret) {
    perror("setsockopt addr");
    return;
  }
}

void Acceptor::setReusePort() {
  int on = 1;
  int ret = setsockopt(_socket.fd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
  if (ret) {
    perror("setsockopt port:");
    return;
  }
}

void Acceptor::bind() {
  int ret =
      ::bind(_socket.fd(), (struct sockaddr *)_InetAdress.getInetAddressPtr(),
             sizeof(struct sockaddr));
  if (ret) {
    perror("::bind:");
    return;
  }
}

void Acceptor::listen() {
  int ret = ::listen(_socket.fd(), 128);
  if (ret) {
    perror("listen");
    return;
  }
}

int Acceptor::accept() {
  int connfd = ::accept(_socket.fd(), nullptr, nullptr);
  if (-1 == connfd) {
    perror("accept");
    return -1;
  }
  return connfd;
}
int Acceptor::fd() { return _socket.fd(); }
