#include "httpTask.h"

httpTask::httpTask(const string ip, HttpTaskCallBack &&httpcb)
    : _ip(ip), _httpcb(httpcb) {
  _httpTask = WFTaskFactory::create_http_task(_ip.c_str(), 5, 0, _httpcb);
  protocol::HttpRequest *req = _httpTask->get_req();
}
httpTask::~httpTask() {}
void httpTask::httpStar() { _httpTask->start(); }
