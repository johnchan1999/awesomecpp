#include "TcpConnection.h"
#include "EventLoop.h"
#include <iostream>
#include <sstream>
#include <sys/socket.h>
using std::cout;
using std::endl;
using std::ostringstream;
TcpConnection::TcpConnection(int fd, EventLoop *loop)
    : _socketIO(fd), _socket(fd), _localAddr(getLocalAddr()),
      _peerAddr(getPeerAddr()), _loop(loop) {}

TcpConnection::~TcpConnection() {}

void TcpConnection::send(const string &msg) {
  _socketIO.writen(msg.c_str(), msg.size());
}

string TcpConnection::receive() {
  char buf[40960] = {0};
  _socketIO.readLine(buf, sizeof(buf));
  return string(buf);
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

void TcpConnection::setNewConnectCallBack(TcpConnectionCallBack &cb) {
  _onTcpConnect = cb;
}

void TcpConnection::setMessageCallBack(TcpConnectionCallBack &cb) {
  _onTcpMessage = cb;
}
void TcpConnection::setCloseCallBack(TcpConnectionCallBack &cb) {
  _onTcpClose = cb;
}

void TcpConnection::handleNewConnectCallBack() {
  if (_onTcpConnect) {
    _onTcpConnect(shared_from_this());
  } else {
    cout << "_onTcpConnect == nullptr" << endl;
  }
}

void TcpConnection::handleMessageCallBack() {
  if (_onTcpMessage) {
    _onTcpMessage(shared_from_this());
  } else {
    cout << "_onTcpMessage == nullptr" << endl;
  }
}

void TcpConnection::handleCloseCallBack() {
  if (_onTcpClose) {
    _onTcpClose(shared_from_this());
  } else {
    cout << "_onTcpClose == nullptr" << endl;
  }
}

bool TcpConnection::isClose() {
  char buf[10] = {0};
  int ret = ::recv(_socket.fd(), buf, sizeof(buf), MSG_PEEK);
  return (0 == ret);
}

void TcpConnection::sendInLoop(const string &msg) {
  _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
}
