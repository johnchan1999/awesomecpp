#ifndef __TASK_H__
#define __TASK_H__
#include "Token.h"
#include <algorithm>
#include <string>
#include <wfrest/HttpServer.h>
#include <wfrest/json.hpp>
#include <workflow/WFTask.h>
using std::string;
using namespace wfrest;

class Task {
public:
  Task();
  ~Task();

  void StaticResourceSignUp(const HttpReq *, HttpResp *resp);

  void StaticResourceSignIn(const HttpReq *, HttpResp *resp);

  void StaticResourceHome(const HttpReq *, HttpResp *resp);

  void StaticResourceJs(const HttpReq *, HttpResp *resp);

  void StaticResourceJpeg(const HttpReq *, HttpResp *resp);

  void StaticResourceUpload(const HttpReq *, HttpResp *resp);

  // void StaticResourceUploadFile(const HttpReq *, HttpResp *resp);

  void RegisterSignIn(const HttpReq *req, HttpResp *resp, SeriesWork *series);

  void RegisterSignInMysql(WFMySQLTask *mysqltask, HttpResp *resp, string name,
                           string encodePassword, SeriesWork *series);

  void RegisterSignInRedis(WFRedisTask *redistask);

  void LoginSignIn(const HttpReq *req, HttpResp *resp, SeriesWork *series);

  void LoginSignInRedis(WFRedisTask *redistask, HttpResp *resp, string name,
                        string encodePassword, string salt_value);
  void LoginSignInMysql(WFMySQLTask *mysqltask, HttpResp *resp, string name,
                        string encodePassword, string salt_value);

  void UserInfo(const HttpReq *req, HttpResp *resp, SeriesWork *series);

  void UserInfoMysql(WFMySQLTask *mysqltask);

  void FileRequire(const HttpResp *req, HttpResp *resp, SeriesWork *series);

  void FileRequireMysql(WFMySQLTask *mysqltask);

  void FileUpload(const HttpReq *req, HttpResp *resp, SeriesWork *series);

  void FileDownLoad(const HttpReq *req, HttpResp *resp);

private:
  string salt(string &password);
};

#endif
