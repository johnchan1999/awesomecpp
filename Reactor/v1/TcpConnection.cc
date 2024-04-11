#include "TcpConnection.h"
#include <iostream>
#include <sstream>
#include <sys/socket.h>
using std::ostringstream;
TcpConnection::TcpConnection(int fd)
    : _socketIO(fd), _socket(fd), _localAddr(getLocalAddr()),
      _peerAddr(getPeerAddr()) {}

TcpConnection::~TcpConnection() {}

void TcpConnection::send(const string &msg) {
  _socketIO.writen(msg.c_str(), msg.size());
}

string TcpConnection::receive() {
  char buf[40960] = {0};
  _socketIO.readn(buf, sizeof(buf));
  return buf;
}

string TcpConnection::toString() {
  ostringstream oss;
  oss << _localAddr.ip() << ":" << _localAddr.port() << "---->"
      << _peerAddr.ip() << ":" << _peerAddr.port();

  return oss.str();
}
InetAddress TcpConnection::getLocalAddr() {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr);
  // 获取本端地址的函数getsockname
  int ret = getsockname(_socket.fd(), (struct sockaddr *)&addr, &len);
  if (-1 == ret) {
    perror("getsockname");
  }

  return InetAddress(addr);
}

// 获取对端的网络地址信息
InetAddress TcpConnection::getPeerAddr() {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr);
  // 获取对端地址的函数getpeername
  int ret = getpeername(_socket.fd(), (struct sockaddr *)&addr, &len);
  if (-1 == ret) {
    perror("getpeername");
  }

  return InetAddress(addr);
}
