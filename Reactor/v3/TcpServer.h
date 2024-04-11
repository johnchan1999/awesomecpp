#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"

class TcpServer {
public:
  TcpServer(string ip, unsigned short port);

  ~TcpServer();

  void start();
  void stop();

  void setAllCallBack(TcpConnectionCallBack &&cb1, TcpConnectionCallBack &&cb2,
                      TcpConnectionCallBack &&cb3);

private:
  Acceptor _acceptor;
  EventLoop _eventloop;
};

#endif // !DEBUG __TCPSERVER_H__
