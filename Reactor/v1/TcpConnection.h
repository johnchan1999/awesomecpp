#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"

class TcpConnection {
public:
  explicit TcpConnection(int fd);
  ~TcpConnection();

  void send(const string &msg);
  string receive();
  string toString();

private:
  InetAddress getLocalAddr();
  InetAddress getPeerAddr();

private:
  SocketIO _socketIO;
  Socket _socket;
  InetAddress _localAddr;
  InetAddress _peerAddr;
};

#endif
