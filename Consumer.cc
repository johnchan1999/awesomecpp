#include "Consumer.h"
#include "TaskQueue.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

Consumer::Consumer(TaskQueue &taskQue) : _taskQue(taskQue) {}

Consumer::~Consumer() {}

void Consumer::process() {
  int cnt = 20;
  while (cnt--) {
    int number = _taskQue.pop();
    cout << "<<Consumer consume number = " << number << endl;
    sleep(1);
  }
}
