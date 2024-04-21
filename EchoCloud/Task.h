#ifndef __TASK_H__
#define __TASK_H__
#include "Token.h"
#include <algorithm>
#include <string>
#include <wfrest/HttpServer.h>
#include <wfrest/json.hpp>
#include <workflow/WFTask.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include "Hash.h"
#include "unixHeader.h"
#include <workflow/WFTaskFactory.h>
#include <workflow/RedisMessage.h>
using std::string;
using namespace wfrest;

class Task
{
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

  void RegisterSignInRedis(WFRedisTask *redistask, HttpResp *resp);

  void LoginSignIn(const HttpReq *req, HttpResp *resp, SeriesWork *series);

  void LoginSignInRedis(WFRedisTask *redistask, HttpResp *resp, string name,
                        string encodePassword, string salt_value);
  void LoginSignInMysql(WFMySQLTask *mysqltask, HttpResp *resp, string name,
                        string encodePassword, string salt_value);

  void UserInfo(const HttpReq *req, HttpResp *resp, SeriesWork *series);

  void UserInfoMysql(WFMySQLTask *mysqltask, HttpResp *resp, string &name);

  void FileRequire(const HttpResp *req, HttpResp *resp, SeriesWork *series);

  void FileRequireMysql(WFMySQLTask *mysqltask);

  void FileUpload(const HttpReq *req, HttpResp *resp, SeriesWork *series);
  void FileUploadRedis(WFRedisTask *redistask, const HttpReq *req, HttpResp *resp, string &tokenStr);
  void FileDownLoad(const HttpReq *req, HttpResp *resp);

  // string salt(string &password);
private:
  std::string generate_salt();
  std::string hash_password(const std::string &password, const std::string &salt);
  std::string generate_salt1(const std::string &password);
};

#endif
