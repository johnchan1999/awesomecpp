#include "TaskQueue.h"
#include "autoMutexGuard.h"
TaskQueue::TaskQueue(const int &queuesize)
    : _queuesize(queuesize), _mutex(), _queue(), _noEmpty(_mutex),
      _noFull(_mutex) {}

TaskQueue::~TaskQueue() {}

int TaskQueue::pop() {
  autoMutexGuard autolock(_mutex);
  while (isempty()) {
    _noEmpty.wait();
  }
  int temp = _queue.front();
  _queue.pop();
  _noFull.notify();
  return temp;
}

void TaskQueue::push(const int val) {
  autoMutexGuard autolock(_mutex);
  while (isfull()) {
    _noFull.wait();
  }
  _queue.push(val);
  _noEmpty.notify();
}

bool TaskQueue::isfull() { return _queue.size() == _queuesize; }
bool TaskQueue::isempty() { return _queue.size() == 0; }
