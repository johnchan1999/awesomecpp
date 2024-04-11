#include "MutexLock.h"
#include <stdio_ext.h>

MutexLock::MutexLock() {
  int ret = pthread_mutex_init(&_mutex, nullptr);
  if (ret) {
    perror("pthread_mutex_init failed");
  }
}
MutexLock::~MutexLock() {
  int ret = pthread_mutex_destroy(&_mutex);
  if (ret) {
    perror("pthread_mutex_destory failed!");
  }
}
void MutexLock::lock() {
  int ret = pthread_mutex_lock(&_mutex);
  if (ret) {
    perror("pthread_mutex_lock failed");
  }
}

void MutexLock::unlock() {
  int ret = pthread_mutex_unlock(&_mutex);
  if (ret) {
    perror("pthread_mutex_unlock failed");
  }
}

void MutexLock::trylock() {
  int ret = pthread_mutex_trylock(&_mutex);
  if (ret) {
    perror("pthread_mutex_trylock failed!");
  }
}

pthread_mutex_t *MutexLock::getMutexLockPtr() { return &_mutex; }
