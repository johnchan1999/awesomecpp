#ifndef __ECHOCLOUD_H__
#define __ECHOCLOUD_H__
#include "Task.h"
#include <wfrest/HttpServer.h>
#include <workflow/WFFacilities.h>
class EchoCloud {
public:
  EchoCloud(int cnt, Task &task);

  ~EchoCloud();

  void start(unsigned short port);

  void loadModules();

private:
  void loadStaticResourceModule();
  void loadUserRegisterModule();
  void loadUserLoginModule();
  void loadUserInfoModule();
  void loadFileQueryModule();
  void loadFileUploadModule();
  void loadFileDownloadModule();

private:
  WFFacilities::WaitGroup _waitGroup;
  wfrest::HttpServer _HttpServer;
  Task _task;
};

#endif
