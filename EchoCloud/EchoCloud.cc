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
// using namespace std::placeholders;

EchoCloud::EchoCloud(int cnt, Task *task) : _waitGroup(cnt), _task(task) {}
EchoCloud::~EchoCloud() {}

void EchoCloud::start(unsigned short port)
{

  if (_HttpServer.track().start(port) == 0)
  {
    _HttpServer.list_routes();
    _waitGroup.wait();
    _HttpServer.stop();
  }
  else
  {
    cout << "httptask start failed" << "\n";
  }
}

void EchoCloud::loadModules()
{

  loadStaticResourceModule();
  loadUserRegisterModule();
  loadUserLoginModule();
  loadUserInfoModule();
  loadFileQueryModule();
  loadFileUploadModule();
  loadFileDownloadModule();
}

void EchoCloud::loadStaticResourceModule()
{
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

void EchoCloud::loadUserRegisterModule()
{
  _HttpServer.POST(
      "/user/signup",
      function<void(const HttpReq *, HttpResp *, SeriesWork *)>(
          bind(&Task::RegisterSignIn, _task, std::placeholders::_1,
               std::placeholders::_2, std::placeholders::_3)));
}

void EchoCloud::loadUserLoginModule()
{
  _HttpServer.POST("/user/signin",
                   function<void(const HttpReq *, HttpResp *, SeriesWork *)>(
                       bind(&Task::LoginSignIn, _task, std::placeholders::_1,
                            std::placeholders::_2, std::placeholders::_3)

                           ));
}

void EchoCloud::loadUserInfoModule()
{
  _HttpServer.GET("/user/info",
                  function<void(const HttpReq *, HttpResp *, SeriesWork *)>(
                      bind(&Task::UserInfo, _task, std::placeholders::_1,
                           std::placeholders::_2, std::placeholders::_3)));
}

void EchoCloud::loadFileQueryModule()
{
  _HttpServer.POST("/file/query", [](const HttpReq *req, HttpResp *resp, SeriesWork *series)
                   {
        //1. 解析请求: 查询词
        string username = req->query("username");
        string tokenStr = req->query("token");
        cout << "username:" << username << "\n";
        cout << "token:" << tokenStr << "\n";
        //2. 解析请求： 消息体
        string limitCnt = req->form_kv()["limit"];

        string mysqlurl("mysql://root:123@localhost");
        auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 1, 
        [=](WFMySQLTask * mysqltask){
            //...检测
            using namespace protocol;
            auto mysqlResp = mysqltask->get_resp();
            MySQLResultCursor cursor(mysqlResp);
            if(cursor.get_cursor_status() == MYSQL_STATUS_GET_RESULT) {
                //读操作,获取用户的
                using std::vector;
                vector<vector<MySQLCell>> matrix;
                cursor.fetch_all(matrix);
                using Json = nlohmann::json;
                Json msgArr;
                for(size_t i = 0; i < matrix.size(); ++i) {
                    Json row;
                    row["FileHash"] = matrix[i][0].as_string();
                    row["FileName"] = matrix[i][1].as_string();
                    row["FileSize"] = matrix[i][2].as_ulonglong();
                    row["UploadAt"] = matrix[i][3].as_datetime();
                    row["LastUpdated"] = matrix[i][4].as_datetime();
                    msgArr.push_back(row);//在数组中添加一个元素,使用push_back即可
                }
                resp->String(msgArr.dump());
            } else {
                //没有读取到正确的信息
                resp->String("error");
            }
        });
        string sql("select file_sha1, file_name, file_size, upload_at, last_update from echocloud.tbl_user_file where user_name = '");
        sql += username + "' limit " + limitCnt;
        cout << "\nsql:\n"  << sql << "\n";
        mysqlTask->get_req()->set_query(sql);
        series->push_back(mysqlTask); });
}
void EchoCloud::loadFileUploadModule()
{
  //
  _HttpServer.POST("/file/upload", function<void(const HttpReq *, HttpResp *, SeriesWork *)>(
                                       std::bind(&Task::FileUpload, _task, std::placeholders::_1,
                                                 std::placeholders::_2, std::placeholders::_3)));
}
void EchoCloud::loadFileDownloadModule()
{
  _HttpServer.POST("/file/downloadurl", function<void(const HttpReq *, HttpResp *)>(
                                            bind(&Task::FileDownLoad, _task, std::placeholders::_1,
                                                 std::placeholders::_2)));
}
