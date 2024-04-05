#include "Thread.h"
#include <iostream>
#include <stdio.h>

using std::cout;
using std::endl;

Thread::Thread(ThreadCallBack &&cb)
    : _thid(0), _isRunning(false), _cb(std::move(cb)) {}

Thread::~Thread() {}

void Thread::start() {
  int ret = pthread_create(&_thid, nullptr, threadFunc, this);
  if (ret) {
    perror("pthread_create");
    return;
  }

  _isRunning = true;
}

void Thread::stop() {
  if (_isRunning) {
    int ret = pthread_join(_thid, nullptr);
    if (ret) {
      perror("pthread_join");
      return;
    }

    _isRunning = false;
  }
}

void *Thread::threadFunc(void *arg) {
  Thread *pth = static_cast<Thread *>(arg);
  if (pth) {
    pth->_cb();
  } else {
    cout << "nullptr == pth" << endl;
  }

  pthread_exit(nullptr);
}
