#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include <pthread.h>

class MutexLock {
public:
  MutexLock();
  virtual ~MutexLock();
  void lock();
  void unlock();
  void trylock();
  pthread_mutex_t *getMutexLockPtr();

private:
  pthread_mutex_t _mutex;
};

#endif
