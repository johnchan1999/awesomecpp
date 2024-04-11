#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <functional>
#include <memory>
using std::function;
class TcpConnection;
class EventLoop;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallBack = function<void(const TcpConnectionPtr &)>;
class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
  explicit TcpConnection(int fd, EventLoop *loop);
  ~TcpConnection();

  void send(const string &msg);
  string receive();
  bool isClose();
  string toString();

  void setNewConnectCallBack(TcpConnectionCallBack &cb);
  void setMessageCallBack(TcpConnectionCallBack &cb);
  void setCloseCallBack(TcpConnectionCallBack &cb);

  void handleNewConnectCallBack();
  void handleMessageCallBack();
  void handleCloseCallBack();

  void sendInLoop(const string &msg);

private:
  InetAddress getLocalAddr();
  InetAddress getPeerAddr();

private:
  SocketIO _socketIO;
  Socket _socket;
  InetAddress _localAddr;
  InetAddress _peerAddr;

  TcpConnectionCallBack _onTcpConnect;
  TcpConnectionCallBack _onTcpMessage;
  TcpConnectionCallBack _onTcpClose;

  EventLoop *_loop;
};

#endif
