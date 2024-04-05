#include "Consumer.h"
#include "Producer.h"
#include "TaskQueue.h"
#include "Thread.h"

using std::bind;
void test() {
  TaskQueue taskQue(10);
  Producer pro(taskQue);
  Consumer con(taskQue);
  Thread mth1(bind(&Producer::process, &pro));
  Thread mth2(bind(&Consumer::process, &con));

  mth1.start();
  mth2.start();

  mth1.stop();
  mth2.stop();
}

int main(int argc, char *argv[]) {
  test();
  return 0;
}
