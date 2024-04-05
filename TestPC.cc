#include "Consumer.h"
#include "Producer.h"
#include "TaskQueue.h"
#include "Thread.h"
#include <memory>
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
void test1() {
  TaskQueue taskQue(20);
  Producer pro(taskQue);
  Consumer con(taskQue);
  std::shared_ptr<Thread> mth1(new Thread(bind(&Producer::process, &pro)));
  std::unique_ptr<Thread> mth2(new Thread(bind(&Consumer::process, &con)));

  mth1->start();
  mth2->start();

  mth1->stop();
  mth2->stop();
}
int main(int argc, char *argv[]) {
  test1();
  return 0;
}
