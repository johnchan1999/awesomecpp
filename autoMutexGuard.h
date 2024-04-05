#ifndef __AUTOMUTEXGUATD_H__
#define __AUTOMUTEXGUATD_H__
#include "MutexLock.h"

class autoMutexGuard {
public:
  autoMutexGuard(MutexLock &mutex);
  ~autoMutexGuard();

private:
  MutexLock &_mutex;
};

#endif
