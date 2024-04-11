#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__
#include "Condition.h"
#include "MutexLock.h"
#include <queue>
using std::queue;

class TaskQueue {
public:
  TaskQueue(const int &queuesize);
  ~TaskQueue();

  void push(const int val);
  int pop();
  bool isfull();
  bool isempty();

private:
  int _queuesize;
  MutexLock _mutex;
  queue<int> _queue;
  Condition _noEmpty;
  Condition _noFull;
};

#endif
