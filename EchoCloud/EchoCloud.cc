#include "EchoCloud.h"
#include "Task.h"
#include <functional>
#include <iostream>
#include <wfrest/json.hpp>
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>

using std::cout;
using namespace wfrest;
using std::bind;
// using std::endl;
using std::function;
using std::string;
// using std::vector;
//using namespace std::placeholders;

EchoCloud::EchoCloud(int cnt, Task * task) : _waitGroup(cnt), _task(task) {}
EchoCloud::~EchoCloud() {}

void EchoCloud::start(unsigned short port) {

  if (_HttpServer.track().start(port) == 0) {
    _HttpServer.list_routes();
    _waitGroup.wait();
    _HttpServer.stop();
  } else {
    cout << "httptask start failed" << "\n";
  }
}

void EchoCloud::loadModules() {

  loadStaticResourceModule();
  loadUserRegisterModule();
  loadUserLoginModule();
  loadUserInfoModule();
  loadFileQueryModule();
  loadFileUploadModule();
  loadFileDownloadModule();
}

void EchoCloud::loadStaticResourceModule() {
  _HttpServer.GET("/user/signup",
                  std::function<void(const HttpReq *, HttpResp *)>(
                      std::bind(&Task::StaticResourceSignUp, _task,
                                std::placeholders::_1, std::placeholders::_2)));
  _HttpServer.GET("/static/view/signup.html",
                  function<void(const HttpReq *, HttpResp *)>(
                      bind(&Task::StaticResourceSignIn, _task,
                           std::placeholders::_1, std::placeholders::_2)));

  _HttpServer.GET("static/view/home.html",
                  function<void(const HttpReq *, HttpResp *)>(
                      bind(&Task::StaticResourceHome, _task,
                           std::placeholders::_1, std::placeholders::_2)));

  _HttpServer.GET("static/js/auth.js",
                  function<void(const HttpReq *, HttpResp *)>(
                      bind(&Task::StaticResourceJs, _task,
                           std::placeholders::_1, std::placeholders::_2)));

  _HttpServer.GET("static/img/avatar.jpeg",
                  function<void(const HttpReq *, HttpResp *)>(
                      bind(&Task::StaticResourceJpeg, _task,
                           std::placeholders::_1, std::placeholders::_2)));
  _HttpServer.Static("file/upload_files", "static/view/upload_files");
}

void EchoCloud::loadUserRegisterModule() {
  _HttpServer.POST(
      "/user/signup",
      function<void(const HttpReq *, HttpResp *, SeriesWork *)>(
          bind(&Task::RegisterSignIn, _task, std::placeholders::_1,
               std::placeholders::_2, std::placeholders::_3)));
}

void EchoCloud::loadUserLoginModule() {
  _HttpServer.POST("/user/signin",
                   function<void(const HttpReq *, HttpResp *, SeriesWork *)>(
                       bind(&Task::LoginSignIn, _task, std::placeholders::_1,
                            std::placeholders::_2, std::placeholders::_3)

                           ));
}

void EchoCloud::loadUserInfoModule() {}

void EchoCloud::loadFileQueryModule(){}
void EchoCloud::loadFileUploadModule(){}
void EchoCloud::loadFileDownloadModule(){}
