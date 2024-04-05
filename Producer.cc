#include "Producer.h"
#include "TaskQueue.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using std::cout;
using std::endl;

Producer::Producer(TaskQueue &taskQue) : _taskQue(taskQue) {}

Producer::~Producer() {}

void Producer::process() {
  ::srand(::clock());
  int cnt = 20;
  while (cnt--) {
    int number = ::rand() % 100;
    _taskQue.push(number);
    cout << ">>Producer produce number = " << number << endl;
    sleep(1);
  }
}
