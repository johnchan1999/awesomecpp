#ifndef __THREAD_H__
#define __THREAD_H__
#include <functional>
#include <pthread.h>
using std::function;
class Thread {
  using ThreadCallBack = function<void()>;

public:
  Thread(ThreadCallBack &&cb);
  ~Thread();

  void start();
  void stop();

private:
  static void *threadFunc(void *arg);

private:
  pthread_t _thid;
  bool _isRunning;
  ThreadCallBack _cb;
};

#endif
