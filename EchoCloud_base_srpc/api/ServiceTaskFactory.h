#ifndef __ISERVICETASK_H__
#define __ISERVICETASK_H_

#include <wfrest/HttpServer.h>
#include <string>
#include "../signup/signup.srpc.h"
#include "../login/login.srpc.h"
#include "../userinfo/userinfo.srpc.h"
#include "../filequery/filequery.srpc.h"
using namespace wfrest;
using namespace std;
using namespace srpc;

class IServiceTask
{
public:
    virtual void execute(HttpResp *http_resp, SeriesWork *series) = 0; // 纯虚函数
    virtual ~IServiceTask() {}
};

class SignupServiceTask : public IServiceTask
{
public:
    void execute(HttpResp *http_resp, SeriesWork *series) override;
    void signup_done(::signup::RespSignup *response, srpc::RPCContext *context, HttpResp *http_resp);
};

class LoginServiceTask : public IServiceTask
{
public:
    void execute(HttpResp *http_resp, SeriesWork *series) override;
    void login_done(::login::ResqLogin *response, srpc::RPCContext *context, HttpResp *http_resp);
};

class UserinfoServiceTask : public IServiceTask
{
public:
    void execute(HttpResp *http_resp, SeriesWork *series) override;
    // void userinfo_done(::userinfo::ReqUserinfo *response, srpc::RPCContext *context);
};
class FilequeryServiceTask : public IServiceTask
{
public:
    void execute(HttpResp *http_resp, SeriesWork *series) override;
    void filequery_done(::filequery::RespFilequery *response, srpc::RPCContext *context, HttpResp *resp);
};

class ServiceTaskFactory
{
private:
    std::unordered_map<std::string, std::function<IServiceTask *()>> task_map;

public:
    ServiceTaskFactory();
    IServiceTask *createServiceTask(const std::string &service_name);
};

#endif