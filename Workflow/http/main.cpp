#include "httpTask.h"
#include <signal.h>

static WFFacilities::WaitGroup gWaitGroup(1);

void handler(int signum) {
  std::cout << "signum = " << signum << "\n";
  gWaitGroup.done();
}

void MyTask(WFHttpTask *httpTask) {
  protocol::HttpRequest *req = httpTask->get_req();
  protocol::HttpResponse *resp = httpTask->get_resp();
  int state = httpTask->get_state();
  int error = httpTask->get_error();
  switch (state) {
  case WFT_STATE_SYS_ERROR:
    cout << "system error:" << strerror(error) << "\n";
    break;
  case WFT_STATE_DNS_ERROR:
    cout << "dns error:" << gai_strerror(error) << "\n";
    break;
  case WFT_STATE_SUCCESS:
    break;
  }
  if (state == WFT_STATE_SUCCESS) {
    cout << "SUCCESS\n";
  } else {
    cout << "FAILED\n";
    return;
  }

  string key;
  string val;

  protocol::HttpHeaderCursor cursorReq(req);
  while (cursorReq.next(key, val)) {
    cout << "Request key = " << key << ", value = " << val << "\n";
  }

  protocol::HttpHeaderCursor cursorResp(resp);
  while (cursorResp.next(key, val)) {
    cout << "response key = " << key << ", value =" << val << "\n";
  }
  const void *body;
  size_t size;
  resp->get_parsed_body(&body, &size);
  cout << string((char *)body, size) << "\n";
}

int main() {
  signal(SIGINT, handler);
  const string ip("http://103.185.249.166:8888");

  const string ip1("http://www.baidu.com");
  httpTask hptask(ip1, std::bind(&MyTask, std::placeholders::_1));
  hptask.httpStar();

  gWaitGroup.wait();
}
