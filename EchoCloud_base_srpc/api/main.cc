#include "EchoCloud.h"
#include "Task.h"
#include <iostream>
int main()
{
  Task *task = new Task();
 // ClientConfiguration conf;
  //conf.maxConnections = 10;
 // conf.requestTimeoutMs = 1000;
 // conf.connectTimeoutMs = 1000;
  EchoCloud echocloudServer(1, task);
  echocloudServer.loadModules();
  echocloudServer.start(1234);
  return 0;
}
