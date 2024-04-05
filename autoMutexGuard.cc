#include "autoMutexGuard.h"

autoMutexGuard::autoMutexGuard(MutexLock &mutex) : _mutex(mutex) {
  mutex.lock();
}
autoMutexGuard::~autoMutexGuard() { _mutex.unlock(); }
