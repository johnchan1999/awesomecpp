#include <iostream>
#include <string>
#include <sys/signal.h>
#include <vector>
#include <wfrest/HttpServer.h>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>
#include <workflow/WFFacilities.h>
#include <workflow/WFHttpServer.h>
using std::cout;
using std::string;
struct SeriesContext {
  protocol::HttpResponse *resp;
  char buf[4096];
};
static WFFacilities::WaitGroup gWaitGroup(1);
// 阻塞主线程，直到按下ctrl+c
void handler(int signum) {
  printf("signum = %d\n", signum);
  gWaitGroup.done(); // 计数器减一
}
void process() {}
int main() {
  signal(SIGINT, handler);
  WFHttpServer server(process);
  server.start(1234);
  gWaitGroup.wait();
  server.stop();
}
