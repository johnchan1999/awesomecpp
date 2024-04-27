#include "Task.h"
#include "../signup/signup.srpc.h"
#include <crypt.h>
#include <iostream>
#include <random>
#include <vector>
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include "ServiceTaskFactory.h"

using namespace srpc;

using std::cout;
using std::vector;

Task::Task() {}
Task::~Task() {}

void Task::StaticResourceSignUp(const HttpReq *, HttpResp *resp)
{
  resp->File("static/view/signup.html");
}

void Task::StaticResourceSignIn(const HttpReq *, HttpResp *resp)
{
  resp->File("static/view/signin.html");
}

void Task::StaticResourceSignUpOk(const HttpReq *, HttpResp *resp)
{
  resp->File("static/view/signup.html");
}

void Task::StaticResourceSigninOk(const HttpReq *, HttpResp *resp)
{
  resp->File("static/view/signin.html");
}

void Task::StaticResourceHome(const HttpReq *, HttpResp *resp)
{
  resp->File("static/view/home.html");
}

void Task::StaticResourceJs(const HttpReq *, HttpResp *resp)
{
  resp->File("static/js/auth.js");
}

void Task::StaticResourceJpeg(const HttpReq *, HttpResp *resp)
{
  resp->File("static/img/avatar.jpeg");
}

void Task::StaticResourceUpload(const HttpReq *, HttpResp *resp)
{
  resp->File("static/view/index.html");
}

void Task::RegisterSignIn(const HttpReq *req, HttpResp *http_resp,
                          SeriesWork *series)
{

  if (req->content_type() == wfrest::APPLICATION_URLENCODED)
  {
    auto formkv = req->form_kv();
    string name = formkv["username"];
    string password = formkv["password"];
    cout << "name =" << name << ",password =" << password << "\n";
    SeriesContext *context_ptr = new SeriesContext;
    context_ptr->password = password;
    context_ptr->username = name;
    string service_name = "SignupService1";
    context_ptr->service_name = service_name;
    series->set_context(context_ptr);
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    WFHttpTask *httpTask = WFTaskFactory::create_http_task(
        "http://1.13.79.29:8500/v1/agent/services", 10, 0,
        bind(&Task::ConsulCallback, this, std::placeholders::_1, http_resp, series));
    httpTask->get_req()->set_method("GET");
    series->push_back(httpTask);
  }
}

void removeLastDigit(std::string &str)
{
  if (!str.empty() && std::isdigit(static_cast<unsigned char>(str.back())))
  {
    str.pop_back(); // 如果最后一个字符是数字，则删除
  }
}

void Task::ConsulCallback(WFHttpTask *httpTask, HttpResp *http_resp, SeriesWork *series)
{
  SeriesContext *context_ptr = (SeriesContext *)series->get_context();
  string service_name = context_ptr->service_name;
  protocol::HttpResponse *resp = httpTask->get_resp();
  using Json = nlohmann::json;
  const void *body;
  size_t size;
  resp->get_parsed_body(&body, &size);
  string body_str((const char *)body, size);
  cout << "body_str:" << body_str << "\n";
  Json info = Json::parse(body_str);
  Json Service = info[service_name];
  const string ip = Service["Address"];
  unsigned port = Service["Port"];
  cout << "ip:" << ip << ",port:" << port << "\n";
  const char *ip_c = ip.c_str();
  cout << "ip_c:" << ip_c << "\n";

  context_ptr->ip_c = ip_c;
  context_ptr->port = port;

  removeLastDigit(service_name);

  ServiceTaskFactory srpcFactory;
  std::unique_ptr<IServiceTask> service_task(srpcFactory.createServiceTask(service_name));
  if (service_task)
  {
    service_task->execute(http_resp, series);
  }
  else
  {
    cout << "Error handling: No corresponding service task for the provided service_name.\n";
  }
}

void Task::LoginSignIn(const HttpReq *req, HttpResp *http_resp, SeriesWork *series)
{
  if (req->content_type() == APPLICATION_URLENCODED)
  {
    // 解析请求获取用户名和密码
    auto formkv = req->form_kv();
    string name = formkv["username"];
    string password = formkv["password"];
    cout << "username =" << name << ", password =" << password << "\n";
    SeriesContext *context_ptr = new SeriesContext;
    context_ptr->password = password;
    context_ptr->username = name;
    string service_name = "LoginService1";
    context_ptr->service_name = service_name;
    series->set_context(context_ptr);

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    WFHttpTask *httpTask = WFTaskFactory::create_http_task(
        "http://1.13.79.29:8500/v1/agent/services", 10, 0,
        bind(&Task::ConsulCallback, this, std::placeholders::_1, http_resp, series));
    httpTask->get_req()->set_method("GET");
    series->push_back(httpTask);
  }
}

using std::endl;
void Task::UserInfo(const HttpReq *req, HttpResp *http_resp, SeriesWork *series)
{

  string name = req->query("username");
  string tokenstr = req->query("token");
  cout << "name:" << name << endl;
  cout << "token" << tokenstr << endl;

  SeriesContext *context_ptr = new SeriesContext;
  context_ptr->username = name;
  context_ptr->token = tokenstr;
  string service_name = "UserinfoService1";
  context_ptr->service_name = service_name;
  series->set_context(context_ptr);

  GOOGLE_PROTOBUF_VERIFY_VERSION;
  WFHttpTask *httpTask = WFTaskFactory::create_http_task(
      "http://1.13.79.29:8500/v1/agent/services", 10, 0,
      bind(&Task::ConsulCallback, this, std::placeholders::_1, http_resp, series));
  httpTask->get_req()->set_method("GET");
  series->push_back(httpTask);
}

void Task::FileRequireMysql(WFMySQLTask *mysqltask, HttpResp *resp)
{
  using namespace protocol;
  auto mysqlResp = mysqltask->get_resp();
  MySQLResultCursor cursor(mysqlResp);
  if (cursor.get_cursor_status() == MYSQL_STATUS_GET_RESULT)
  {
    // 读操作,获取用户的
    using std::vector;
    vector<vector<MySQLCell>> matrix;
    cursor.fetch_all(matrix);
    using Json = nlohmann::json;
    Json msgArr;
    for (size_t i = 0; i < matrix.size(); ++i)
    {
      Json row;
      row["FileHash"] = matrix[i][0].as_string();
      row["FileName"] = matrix[i][1].as_string();
      row["FileSize"] = matrix[i][2].as_ulonglong();
      row["UploadAt"] = matrix[i][3].as_datetime();
      row["LastUpdated"] = matrix[i][4].as_datetime();
      msgArr.push_back(row); 
    }
    resp->String(msgArr.dump());
  }
  else
  {
    
    resp->String("error");
  }
}

void Task::FileRequire(const HttpReq *req, HttpResp *http_resp,
                       SeriesWork *series)
{

  // 1. 解析请求: 查询词
  string username = req->query("username");
  string tokenStr = req->query("token");
  string limitCnt = req->form_kv()["limit"];
  cout << "username:" << username << "\n";
  cout << "token:" << tokenStr << "\n";

  SeriesContext *context_ptr = new SeriesContext;
  context_ptr->username = username;
  context_ptr->token = tokenStr;
  string service_name = "FilequeryService1";
  context_ptr->service_name = service_name;
  context_ptr->limitCnt = limitCnt;
  series->set_context(context_ptr);

  GOOGLE_PROTOBUF_VERIFY_VERSION;
  WFHttpTask *httpTask = WFTaskFactory::create_http_task(
      "http://1.13.79.29:8500/v1/agent/services", 10, 0,
      bind(&Task::ConsulCallback, this, std::placeholders::_1, http_resp, series));
  httpTask->get_req()->set_method("GET");
  series->push_back(httpTask);
}

void Task::FileUpload(const HttpReq *req, HttpResp *resp, SeriesWork *series)
{
  string username = req->query("username");
  string tokenStr = req->query("token");
  cout << "username:" << username << endl;
  cout << "token:" << tokenStr << endl;
  // 2. 对token进行验证
  string redisurl("redis://127.0.0.1:6379");
  auto redisTask = WFTaskFactory::create_redis_task(
      redisurl, 0,
      bind(&Task::FileUploadRedis, this, std::placeholders::_1, req, resp,
           tokenStr));

  redisTask->get_req()->set_request("HGET", {"Token", username});
  series->push_back(redisTask);
}

void Task::FileUploadRedis(WFRedisTask *redistask, const HttpReq *req,
                           HttpResp *resp, string &tokenStr)
{
  // Step 1: Check the Redis task state
  if (redistask->get_state() != WFT_STATE_SUCCESS)
  {
    resp->set_status_code("500 Internal Server Error");
    resp->append_output_body(
        "<html><body><h1>500 Internal Server Error</h1><p>Token validation "
        "failed due to Redis task error.</p></body></html>");
    return;
  }

  protocol::RedisResponse *redis_resp = redistask->get_resp();
  protocol::RedisValue val;
  redis_resp->get_result(val);
  // Step 2: Parse the response from Redis
  if (val.is_nil())
  {

    resp->set_status_code("401 Unauthorized");
    resp->append_output_body("<html><body><h1>401 Unauthorized</h1><p>Invalid "
                             "or missing token.</p></body></html>");
    return;
  }
  // If the token is valid, you would continue with the file upload process.
  // For example, you may want to proceed with file saving, database update,
  // etc. Since these steps would depend on the remaining part of your workflow,
  // they are not included here. You can, however, use the validation result
  // to conditionally execute those steps.
  if (val.is_string())
  {
    cout << "tokenStr:" << tokenStr << endl;
    cout << "val:" << val.string_value() << endl;
    if (val.string_value() == tokenStr)
    {
      cout << "Redis has found tokenStr" << endl;
      string username = req->query("username");
      // 3. 解析请求：消息体
      if (req->content_type() == MULTIPART_FORM_DATA)
      {
        auto form = req->form();
        string filename = form["file"].first;
        string content = form["file"].second;
        // TODO: 由于是同步的并且会阻塞应当更换为IOtask任务

        //  4. 将数据写入服务器本地
        mkdir("tmp", 0755);
        string filepath = "tmp/" + filename;
        cout << "filepath:" << filepath << endl;
        int fd = open(filepath.c_str(), O_CREAT | O_RDWR, 0664);
        if (fd < 0)
        {
          perror("open");
          return;
        }
        write(fd, content.c_str(), content.size());
        close(fd);
        resp->String("upload Success");

        // 5. 生成SHA1值
        Hash hash(filepath);
        string filehash = hash.sha1();
        cout << "filehash:" << filehash << endl;
        // 6.将文件相关信息写入数据库MySQL中
        string mysqlurl("mysql://root:cj19991114@localhost");
        auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 1, nullptr);
        string sql("INSERT INTO "
                   "echocloud.tbl_user_file(user_name,file_sha1,file_size,file_"
                   "name)VALUES('");
        sql += username + "','" + filehash + "', " +
               std::to_string(content.size()) + ",'" + filename + "')";
        cout << "\nsql:\n"
             << sql << endl;
        mysqlTask->get_req()->set_query(sql);
        series_of(redistask)->push_back(mysqlTask);
      }
    }
  }
  // resp->set_status_code("200 OK");
  // resp->append_output_body("<html><body><h1>200 OK</h1><p>Token validated
  // successfully.</p></body></html>");
}
void Task::FileDownLoad(const HttpReq *req, HttpResp *resp)
{
  string filename = req->query("filename");
  cout << "filename: " << filename << endl;
  // TODO: use alibaba/oss/ to generate download link
  //  将下载业务从服务器中分离出去，之后只需要产生一个下载链接就可以了
  //  这要求我们还需要去部署一个下载服务器
  string downloadURL = "http://1.13.79.29:8080/" + filename;
  cout << downloadURL << endl;
  resp->String(downloadURL);
}
string Task::generate_salt()
{
  const std::string salt_chars =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<> dist(0, salt_chars.size() - 1);

  std::string salt = "$6$"; // 使用SHA-512算法的标识
  for (int i = 0; i < 16; i++)
  { // 生成16个字符的盐值
    salt += salt_chars[dist(generator)];
  }
  salt += '$'; // 末尾加上$符号，结束盐值部分

  return salt;
}

string Task::hash_password(const std::string &password,
                           const std::string &salt)
{
  // std::string salt = generate_salt();
  // cout << "salt:" << salt << "\n";

  std::string hashed_password = crypt(password.c_str(), salt.c_str());

  if (hashed_password[0] == '*')
  {
    throw std::runtime_error("Password hashing failed.");
  }

  return hashed_password;
}
std::string Task::generate_salt1(const std::string &password)
{
  // 使用密码的哈希值作为伪随机数生成器的种子
  std::hash<std::string> hasher;
  size_t hashed_password = hasher(password);

  // 将种子转化为字符串
  std::stringstream ss;
  ss << std::hex << hashed_password;

  // 截取特定长度作为盐值，这里假设我们要的盐的长度是8
  std::string salt = ss.str().substr(0, 20);

  return salt;
}
