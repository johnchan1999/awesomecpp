#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
using std::cout;
using std::endl;

EventLoop::EventLoop(Acceptor &acceptor)
    : _epfd(createEpfd()), _evtList(1024), _isRunning(false),
      _acceptor(acceptor), _evtfd(createEventFd()), _mutex() {
  int listenfd = _acceptor.fd();
  addEpoolReadFd(listenfd);
}

EventLoop::~EventLoop() {
  close(_epfd);
  close(_evtfd);
}

void EventLoop::Loop() {
  _isRunning = true;
  while (_isRunning) {
    epoolWait();
  }
}
void EventLoop::unLoop() { _isRunning = false; }
void EventLoop::epoolWait() {

  int nready = 0;
  do {
    nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 7000);

  } while (nready == -1 && errno == EINTR);
  if (nready == -1) {
    perror("epoll_wait return -1");
    return;
  } else if (nready == 0) {
    cout << "---epoll_wait time out ------" << endl;
  } else {
    if (nready == (int)_evtList.size()) {
      _evtList.resize(2 * nready);
    }
    for (int idx = 0; idx < nready; idx++) {
      int fd = _evtList[idx].data.fd;
      if (fd == _acceptor.fd()) {
        if (_evtList[idx].events & EPOLLIN) {
          handConnect();
        }
      } else if (fd == _evtfd) {
        handleRead();
        doPendingFunction();
      } else {
        if (_evtList[idx].events & EPOLLIN) {
          handMessage(fd);
        }
      }
    }
  }
}

void EventLoop::handConnect() {
  int connfd = _acceptor.accept();
  if (connfd < 0) {
    perror("accept failed");
    return;
  }
  addEpoolReadFd(connfd);
  TcpConnectionPtr con(new TcpConnection(connfd, this));
  con->setNewConnectCallBack(_onTcpConnect);
  con->setMessageCallBack(_onTcpMessage);
  con->setCloseCallBack(_onTcpClose);
  _cons.insert(std::make_pair(connfd, con));

  con->handleNewConnectCallBack();
}

void EventLoop::handMessage(int fd) {
  auto it = _cons.find(fd);
  if (it != _cons.end()) {
    bool flag = it->second->isClose();
    if (flag) {
      it->second->handleCloseCallBack();
      delEpoolReadFd(fd);
      _cons.erase(it);
    } else {
      it->second->handleMessageCallBack();
    }
  } else {
    cout << "the connection doesn't exit!" << endl;
    return;
  }
}

int EventLoop::createEpfd() {
  int fd = ::epoll_create(100);
  if (fd < 0) {
    perror("epoll_create failed");
    return fd;
  }
  return fd;
}

void EventLoop::addEpoolReadFd(int fd) {
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fd;

  int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event);
  if (ret < 0) {
    perror("epoll_ctl failed");
    return;
  }
}

void EventLoop::delEpoolReadFd(int fd) {
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fd;

  int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &event);
  if (ret < 0) {
    perror("EPOLL_CTL_DEL failed");
    return;
  }
}

void EventLoop::setNewConnectCallBack(TcpConnectionCallBack &&cb) {
  _onTcpConnect = std::move(cb);
}

void EventLoop::setMessageCallBack(TcpConnectionCallBack &&cb) {
  _onTcpMessage = std::move(cb);
}
void EventLoop::setCloseCallBack(TcpConnectionCallBack &&cb) {
  _onTcpClose = std::move(cb);
}

void EventLoop::wakeup() {
  uint64_t one = 1;
  ssize_t ret = write(_evtfd, &one, sizeof(uint64_t));
  if (ret != sizeof(uint64_t)) {
    perror("wakeup failed");
    return;
  }
}

void EventLoop::runInLoop(Functor &&cb) {
  {
    MutexLockGuard auto_mutex(_mutex);
    _pendings.push_back(std::move(cb));
  }
  wakeup();
}

int EventLoop::createEventFd() {
  int fd = eventfd(10, 0);
  if (fd < 0) {
    perror("eventfd return -1");
    return -1;
  }
  return fd;
}

void EventLoop::handleRead() {
  uint64_t two;
  ssize_t ret = read(_evtfd, &two, sizeof(uint64_t));
  if (ret != sizeof(uint64_t)) {
    perror("handleRead failed");
    return;
  }
}

void EventLoop::doPendingFunction() {
  vector<Functor> temp;
  {
    MutexLockGuard auto_mutex(_mutex);
    temp.swap(_pendings);
  }
  for (auto &cb : temp) {
    cb();
  }
}
