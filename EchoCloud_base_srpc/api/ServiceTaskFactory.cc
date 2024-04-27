#include "ServiceTaskFactory.h"
#include "Task.h"
#include <wfrest/json.hpp>
using std::cout;
void SignupServiceTask::execute(HttpResp *http_resp, SeriesWork *series)
{
    SeriesContext *context_ptr = (SeriesContext *)series->get_context();
    string name = context_ptr->username;
    string password = context_ptr->password;
    const char *ip_c = context_ptr->ip_c;
    unsigned port = context_ptr->port;
    ::signup::SignupService::SRPCClient client(ip_c, port);
    ::signup::ReqSignup signup_req;

    signup_req.set_username(name);
    signup_req.set_password(password);

    client.Signup(&signup_req, series, bind(&SignupServiceTask::signup_done, this, std::placeholders::_1, std::placeholders::_2, http_resp));
}

void SignupServiceTask::signup_done(::signup::RespSignup *response, srpc::RPCContext *context, HttpResp *http_resp)
{
    cout << "code = " << response->code() << "\n";
    cout << "messsage = " << response->message() << "\n";

    if (response->code() == 0)
    {
        cout << "注册成功" << "\n";

        http_resp->String("SUCCESS");
        cout << "已经发送给客户端" << "\n";
    }
    else
    {
        cout << "注册失败" << "\n";
        http_resp->String("FAIL");
    }
}

void LoginServiceTask::execute(HttpResp *http_resp, SeriesWork *series)
{

    SeriesContext *context_ptr = (SeriesContext *)series->get_context();
    string name = context_ptr->username;
    string password = context_ptr->password;
    const char *ip_c = context_ptr->ip_c;
    unsigned port = context_ptr->port;
    ::login::loginService::SRPCClient client(ip_c, port);
    ::login::ReqLogin login_req;

    login_req.set_username(name);
    login_req.set_password(password);

    client.Login(&login_req, series, bind(&LoginServiceTask::login_done, this, std::placeholders::_1, std::placeholders::_2, http_resp));
}

void LoginServiceTask::login_done(::login::ResqLogin *response, srpc::RPCContext *context, HttpResp *http_resp)
{
    cout << "code = " << response->code() << "\n";
    cout << "messsage = " << response->message() << "\n";

    if (response->code() == 0)
    {
        cout << "登录成功" << "\n";
        // 3.2构造一个JSON对象，发送给客户端
        string tokenStr = response->message();
        string name = response->username();
        using Json = nlohmann::json;
        Json msg;
        Json data;
        data["Token"] = tokenStr;
        data["Username"] = name;
        data["Location"] = "/static/view/home.html"; // 跳转到用户中心页面
        msg["data"] = data;
        http_resp->String(msg.dump()); // 序列化之后，发送给客户端
        cout << "已经发送给客户端" << "\n";
    }
    else
    {
        cout << "登录失败" << "\n";
        http_resp->String("FAIL");
    }
}
static void userinfo_done(::userinfo::RespUserinfo *response, srpc::RPCContext *context, HttpResp *http_resp)
{
    cout << "code = " << response->code() << "\n";
    cout << "messsage = " << response->message() << "\n";

    if (response->code() == 0)
    {
        cout << "获取用户信息成功" << "\n";
        // 3.2构造一个JSON对象，发送给客户端
        int code = response->code();
        string messages = response->message();
        string josn_data = response->extr_message();

        http_resp->String(josn_data); // 序列化之后，发送给客户端
    }
    else
    {
        cout << "获取用户信息失败" << "\n";
        http_resp->String("FAIL");
    }
}

void UserinfoServiceTask::execute(HttpResp *http_resp, SeriesWork *series)
{
    SeriesContext *context_ptr = (SeriesContext *)series->get_context();
    string token = context_ptr->token;
    string name = context_ptr->username;
    const char *ip_c = context_ptr->ip_c;
    unsigned port = context_ptr->port;
    ::userinfo::UserInfoService::SRPCClient client(ip_c, port);
    ::userinfo::ReqUserinfo userinfo_req;
    userinfo_req.set_name(name);
    userinfo_req.set_tokenstr(token);
    client.UserInfo(&userinfo_req, series, std::bind(&userinfo_done, std::placeholders::_1, std::placeholders::_2, http_resp));
}

void FilequeryServiceTask::execute(HttpResp *http_resp, SeriesWork *series)
{
    SeriesContext *context_ptr = (SeriesContext *)series->get_context();
    string token = context_ptr->token;
    string name = context_ptr->username;
    string limitCnt = context_ptr->limitCnt;
    const char *ip_c = context_ptr->ip_c;
    unsigned port = context_ptr->port;
    ::filequery::filequeryService::SRPCClient client(ip_c, port);
    ::filequery::ReqFilequery filequery_req;
    filequery_req.set_username(name);
    filequery_req.set_tokenstr(token);
    filequery_req.set_extr_message(limitCnt);

    client.Filequery(&filequery_req, series, std::bind(&FilequeryServiceTask::filequery_done, this, std::placeholders::_1, std::placeholders::_2, http_resp));
}

void FilequeryServiceTask::filequery_done(::filequery::RespFilequery *response, srpc::RPCContext *context, HttpResp *resp)
{
    cout << "code = " << response->code() << "\n";
    cout << "messsage = " << response->message() << "\n";

    if (response->code() == 0)
    {
        cout << "获取文件列表成功" << "\n";
        // 3.2构造一个JSON对象，发送给客户端
        string josn_data = response->extr_message();
        resp->String(josn_data); // 序列化之后，发送给客户端
    }
    else
    {
        cout << "获取文件列表失败" << "\n";
        resp->String("FAIL");
    }
}
ServiceTaskFactory::ServiceTaskFactory()
{
    // 注册所有可能的服务处理任务
    task_map["SignupService"] = []()
    { return new SignupServiceTask(); };
    task_map["LoginService"] = []()
    { return new LoginServiceTask(); };
    task_map["UserinfoService"] = []()
    { return new UserinfoServiceTask(); };
    task_map["FilequeryService"] = []()
    { return new FilequeryServiceTask(); };
}
IServiceTask *ServiceTaskFactory::createServiceTask(const std::string &service_name)
{
    auto it = task_map.find(service_name);
    if (it != task_map.end())
    {
        return it->second(); // 调用 lambda 函数来创建任务实例
    }
    return nullptr;
}