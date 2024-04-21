#include "EchoCloud.h"
#include "Task.h"
#include <iostream>
int main() {
  // Task task;
  Task* task = new Task();
  EchoCloud echocloudServer(1, task);
  echocloudServer.loadModules();
  echocloudServer.start(1234);
  return 0;
}
