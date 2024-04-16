#ifndef __HTTPTASK_H__
#define __HTTPTASK_H__
#include <functional>
#include <iostream>
#include <string>
#include <workflow/HttpMessage.h>
#include <workflow/HttpUtil.h>
#include <workflow/WFFacilities.h>

using std::cout;
using std::function;
using std::string;
using HttpTaskCallBack = function<void(WFHttpTask *)>;
class httpTask {
public:
  httpTask(const string ip, HttpTaskCallBack &&httpcb);
  ~httpTask();
  void httpStar();

private:
  const string _ip;
  std::function<void(WFHttpTask *)> _httpcb;
  WFHttpTask *_httpTask;
};

#endif // !DEBUG
