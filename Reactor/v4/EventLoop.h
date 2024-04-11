#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "MutexLock.h"
#include "TcpConnection.h"
#include <functional>
#include <map>
#include <memory>
#include <sys/eventfd.h>
#include <vector>
using std::function;
using std::map;
using std::shared_ptr;
using std::vector;
class TcpConnection;
class Acceptor;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallBack = function<void(const TcpConnectionPtr &)>;
using Functor = function<void()>;
class EventLoop {
  // using TcpConnectionPtr = shared_ptr<TcpConnection>;
public:
  EventLoop(Acceptor &acceptor);
  ~EventLoop();
  void Loop();
  void unLoop();

  void setNewConnectCallBack(TcpConnectionCallBack &&cb);
  void setMessageCallBack(TcpConnectionCallBack &&cb);
  void setCloseCallBack(TcpConnectionCallBack &&cb);

  void wakeup();
  void runInLoop(Functor &&cb);

private:
  void epoolWait();
  void handConnect();
  void handMessage(int fd);

  int createEpfd();
  void addEpoolReadFd(int fd);
  void delEpoolReadFd(int fd);

  int createEventFd();
  void handleRead();
  void doPendingFunction();

private:
  int _epfd;
  vector<struct epoll_event> _evtList;
  bool _isRunning;
  Acceptor &_acceptor;
  map<int, TcpConnectionPtr> _cons;

  TcpConnectionCallBack _onTcpConnect;
  TcpConnectionCallBack _onTcpMessage;
  TcpConnectionCallBack _onTcpClose;

  int _evtfd;
  vector<Functor> _pendings;
  MutexLock _mutex;
};

#endif // !__EVENTLOOP_H__
