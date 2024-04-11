#include "Condition.h"
#include "MutexLock.h"
#include <stdio.h>
Condition::Condition(MutexLock &mutex) : _mutex(mutex) {
  int ret = pthread_cond_init(&_cond, nullptr);
  if (ret) {
    perror("pthread_cond_init failed");
  }
}
Condition::~Condition() {
  int ret = pthread_cond_destroy(&_cond);
  if (ret) {
    perror("pthread_cond_destory failed!");
  }
}
void Condition::wait() {
  int ret = pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());
  if (ret) {
    perror("pthread_cond_wait failed!");
  }
}

void Condition::notify() {
  int ret = pthread_cond_signal(&_cond);
  if (ret) {
    perror("pthread_cond_signal failed");
  }
}
void Condition::notifyAll() {
  int ret = pthread_cond_broadcast(&_cond);
  if (ret) {
    perror("pthread_cond_broadcast failed!");
  }
}
