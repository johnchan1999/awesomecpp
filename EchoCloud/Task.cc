#include "Task.h"
#include <iostream>
#include <random>
#include <vector>
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
#include <crypt.h>
#include <random>

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

// void Task::StaticResourceUploadFile(const HttpReq *, HttpResp *resp) {}

void Task::RegisterSignIn(const HttpReq *req, HttpResp *resp,
                          SeriesWork *series)
{
  if (req->content_type() == wfrest::APPLICATION_URLENCODED)
  {
    auto formkv = req->form_kv();
    string name = formkv["username"];
    string password = formkv["password"];
    cout << "name =" << name << ",password =" << password << "\n";

    string salt_value;
    salt_value = generate_salt1(password);
    cout << "salt_value=" << salt_value << "\n";
    string encodePassword = hash_password(password, salt_value);
    cout << "mi wen :" << encodePassword << "\n";

    string mysqlurl("mysql://root:cj19991114@localhost");
    auto mysqlTask = WFTaskFactory::create_mysql_task(
        mysqlurl, 1,
        std::bind(&Task::RegisterSignInMysql, this, std::placeholders::_1, resp,
                  name, encodePassword, series));
    string sql("INSERT INTO echocloud.tbl_user(user_name,user_pwd) VALUE ('");
    sql += name + "','" + encodePassword + "')";
    cout << "sql:\n"
         << sql << "\n";
    mysqlTask->get_req()->set_query(sql);
    series->push_back(mysqlTask);
  }
}

void Task::RegisterSignInMysql(WFMySQLTask *mysqltask, HttpResp *resp,
                               string name, string encodePassword,
                               SeriesWork *series)
{
  int state = mysqltask->get_state();
  int error = mysqltask->get_error();
  if (state != WFT_STATE_SUCCESS)
  {
    cout << WFGlobal::get_error_string(state, error) << "\n";
    return;
  }

  auto mysqlResp = mysqltask->get_resp();
  if (mysqlResp->get_packet_type() == MYSQL_PACKET_ERROR)
  {
    cout << "ERROR" << mysqlResp->get_error_code() << ":"
         << mysqlResp->get_error_msg() << "\n";
    resp->String("Signup failed");
    return;
  }

  using namespace ::protocol;
  MySQLResultCursor cursor(mysqlResp);
  if (cursor.get_cursor_status() == MYSQL_STATUS_OK)
  {
    cout << "Query OK." << cursor.get_affected_rows() << "row affected \n";
    resp->String("SUCCESS");
    string redisurl = "redis://127.0.0.1:6379";
    WFRedisTask *redisTask = WFTaskFactory::create_redis_task(
        redisurl, 0,
        bind(&Task::RegisterSignInRedis, this, std::placeholders::_1, resp));
    protocol::RedisRequest *req = redisTask->get_req();
    req->set_request("HSET", {"user", name, encodePassword});
    series->push_back(redisTask);
  }
  else
  {
    resp->String("SignUp failed(mysql)!");
  }
}
// void Task::RegisterSignInRedis(WFRedisTask *redistask) {
//   protocol::RedisResponse *resp = redistask->get_resp();
//   protocol::RedisValue value;
//   resp->get_result(value);
// }
void Task::RegisterSignInRedis(WFRedisTask *redistask, HttpResp *resp)
{
  protocol::RedisResponse *redis_resp = redistask->get_resp();
  protocol::RedisValue value;

  // 获取结果
  int error = redistask->get_state();
  redis_resp->get_result(value);

  // 检查任务状态以及响应结果
  if (error == WFT_STATE_SUCCESS && value.is_int())
  {
    long long result = value.int_value();
    cout << result << "\n";
    if (result == 1)
    {
      // HSET 成功，并且字段是新的
      std::cout << "HSET successful with a new field set." << std::endl;
    }
    else if (result == 0)
    {
      // HSET 成功，但字段已存在，旧值被新值覆盖
      std::cout << "HSET successful with an existing field overwritten."
                << std::endl;
    }
    else
    {
      // 这通常不应该发生，因为 HSET 应该返回 0 或 1
      std::cerr << "HSET returned an unexpected result." << std::endl;
    }
  }
  else
  {
    // 操作失败或返回结果类型不是整数
    std::cerr << "HSET operation failed or return type is not integer."
              << std::endl;
  }
}

void Task::LoginSignIn(const HttpReq *req, HttpResp *resp, SeriesWork *series)
{
  if (req->content_type() == APPLICATION_URLENCODED)
  {
    // 解析请求获取用户名和密码
    auto formkv = req->form_kv();
    string name = formkv["username"];
    string password = formkv["password"];
    cout << "username =" << name << ", password =" << password << "\n";

    string salt_value;
    salt_value = generate_salt1(password);
    cout << "salt_value=" << salt_value << "\n";
    string encodePassword = hash_password(password, salt_value);
    cout << "mi wen :" << encodePassword << "\n";

    // create redistask to save user information

    string redisurl = "redis://127.0.0.1:6379";
    WFRedisTask *redisTask = WFTaskFactory::create_redis_task(
        redisurl, 0,
        bind(&Task::LoginSignInRedis, this, std::placeholders::_1, resp, name,
             encodePassword, salt_value));

    using namespace protocol;
    RedisRequest *req = redisTask->get_req();
    req->set_request("HGET", {"user", name});
    series->push_back(redisTask);
  }
}

void Task::LoginSignInRedis(WFRedisTask *redisTask, HttpResp *resp, string name,
                            string encodePassword, string salt_value)
{

  protocol::RedisRequest *req = redisTask->get_req();
  protocol::RedisResponse *redisresp = redisTask->get_resp();
  protocol::RedisValue value; // 将要用来获取redis返回的结果
  int state = redisTask->get_state();
  int error = redisTask->get_error(); // error只能处理网络连接和系统错误
  switch (state)
  {
  case WFT_STATE_SYS_ERROR:
    cout << "system error:" << strerror(error) << "\n";
    break;
  case WFT_STATE_DNS_ERROR:
    cout << "dns error: " << gai_strerror(error) << "\n";
    break;
  case WFT_STATE_SUCCESS:
    redisresp->get_result(value); // 将redis的结果已经存在value中了
    if (value.is_error())
    {
      cout << "redis error\n";
      state = WFT_STATE_TASK_ERROR; // 任务层面的错误
    }
    break;
  }

  if (state == WFT_STATE_SUCCESS)
  {
    if (value.is_nil())
    {
      cout << "User not found in Redis, checking MySQL..." << "\n";

      string mysqlurl("mysql//:root:cj19991114@localhost");
      auto *mysqlTask = WFTaskFactory::create_mysql_task(
          mysqlurl, 1,
          std::bind(&Task::LoginSignInMysql, this, std::placeholders::_1, resp,
                    name, encodePassword, salt_value));

      string sql("select user_pwd from echocloud.tbl_user where user_name = '");
      sql += name + "' limit 1";
      cout << "sql:\n"
           << sql << "\n";
      mysqlTask->get_req()->set_query(sql);
      series_of(redisTask)->push_back(mysqlTask);
    }
    else if (value.is_string())
    {
      cout << "Found user in Redis, value = " << value.string_value() << "\n";
      if (value.string_value() == encodePassword)
      {
        cout << "Login Success\n";
        Token token(name, salt_value);
        string tokenStr = token.genToken();
        // 3.2构造一个JSON对象，发送给客户端
        using Json = nlohmann::json;
        Json msg;
        Json data;
        data["Token"] = tokenStr;
        data["Username"] = name;
        data["Location"] = "/static/view/home.html"; // 跳转到用户中心页面
        msg["data"] = data;
        resp->String(msg.dump()); // 序列化之后，发送给客户端

        // 将token保存到redis中
        string redisurl = "redis://127.0.0.1:6379";
        WFRedisTask *redisTask1 = WFTaskFactory::create_redis_task(
            redisurl, 0, nullptr);
        protocol::RedisRequest *req = redisTask1->get_req();
        req->set_request("HSET", {"Token", name, tokenStr});
        series_of(redisTask)->push_back(redisTask1);

        // // 3.3 将Token保存到数据库中
        // auto nextTask = WFTaskFactory::create_mysql_task(mysqlurl, 1, nullptr);
        // string sql("REPLACE INTO cloudisk.tbl_user_token(user_name, user_token)VALUES('");
        // sql += username + "', '" + tokenStr + "')";
        // nextTask->get_req()->set_query(sql);
        // series->push_back(nextTask);
      }
      else
      {
        resp->String("Login Failed");
      }
    }
    // ... 其他情况的处理 ...
  }
  else
  {
    cout << "Redis query FAILED\n";
    // // 可能需要在这里添加MySQL查询作为备份
    // auto *mysqlTask = WFTaskFactory::create_mysql_task();
    // series_of(redisTask)->push_back(mysqlTask);
  }

  // if (state == WFT_STATE_SUCCESS) {
  //   cout << "SUCCESS\n";
  // } else {
  //   cout << "FAILED\n";
  //   return;
  // }
  //
  // if (value.is_string()) {
  //   cout << "value is a string, value = " << value.string_value() << "\n";
  // } else if (value.is_array()) { // 结果是一个字符串数组
  //   for (size_t i = 0; i < value.arr_size(); ++i) {
  //     cout << "value is arr, i = " << i
  //          << " , arr[i] = " << value.arr_at(i).string_value() << "\n";
  //   }
  // }
}

void Task::LoginSignInMysql(WFMySQLTask *mysqltask, HttpResp *resp, string name,
                            string encodePassword, string salt_value)
{
  int state = mysqltask->get_state();
  int error = mysqltask->get_error();
  if (state != WFT_STATE_SUCCESS)
  {
    printf("%s\n", WFGlobal::get_error_string(state, error));
    return;
  }
  // 1. 检测SQL语句是否存在语法错误
  auto mysqlResp = mysqltask->get_resp();
  if (mysqlResp->get_packet_type() == MYSQL_PACKET_ERROR)
  {
    printf("ERROR %d: %s\n", mysqlResp->get_error_code(),
           mysqlResp->get_error_msg().c_str());
    resp->String("Singup Failed");
    return;
  }
  using namespace protocol;
  MySQLResultCursor cursor(mysqlResp);
  if (cursor.get_cursor_status() == MYSQL_STATUS_OK)
  {
    // 2. 成功写入数据库了
    printf("Query OK. %llu row affected.\n", cursor.get_affected_rows());
    resp->String("Login Failed");
  }
  else if (cursor.get_cursor_status() == MYSQL_STATUS_GET_RESULT)
  {
    // 3. 读取数据
    vector<vector<MySQLCell>> matrix;
    cursor.fetch_all(matrix);
    string M = matrix[0][0].as_string();
    cout << "M:" << M << "\n";
    if (encodePassword == M)
    {
      // 3.1登录成功的情况, 生成Token信息
      Token token(name, salt_value);
      string tokenStr = token.genToken();
      // 3.2构造一个JSON对象，发送给客户端
      using Json = nlohmann::json;
      Json msg;
      Json data;
      data["Token"] = tokenStr;
      data["Username"] = name;
      data["Location"] = "/static/view/home.html"; // 跳转到用户中心页面
      msg["data"] = data;
      resp->String(msg.dump()); // 序列化之后，发送给客户端

      // 将Token保存在redis中
      string redisurl("redis://127.0.0.1:6379");
      auto nextTask = WFTaskFactory::create_redis_task(redisurl, 0, nullptr);

      nextTask->get_req()->set_request("HSET", {"Token", name, tokenStr});
      series_of(mysqltask)->push_back(nextTask);
      cout << "nexttask has done\n";
      // // 3.3 将Token保存到数据库中
      // string mysqlurl("mysql://root:cj19991114@localhost");
      // auto nextTask = WFTaskFactory::create_mysql_task(mysqlurl, 1, nullptr);
      // string sql("REPLACE INTO cloudisk.tbl_user_token(user_name, "
      //            "user_token)VALUES('");
      // sql += name + "', '" + tokenStr + "')";
      // nextTask->get_req()->set_query(sql);
    }
    else
    {
      // 登录失败的情况
      resp->String("Login Failed");
    }
  }
}

// void Task::LoginSignInMysqlToken(const HttpReq * req,HttpResp * resp,)
using std::endl;
void Task::UserInfo(const HttpReq *req, HttpResp *resp, SeriesWork *series)
{

  string name = req->query("username");
  string tokenstr = req->query("token");
  cout << "name:" << name << endl;
  cout << "token" << tokenstr << endl;

  string mysqlurl("mysql://root:cj19991114@localhost");
  auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 1, bind(&Task::UserInfoMysql, this, std::placeholders::_1, resp, name));
  string sql("select signup_at from echocloud.tbl_user where user_name = '");
  sql += name + "'";
  mysqlTask->get_req()->set_query(sql);
  series->push_back(mysqlTask);
}

void Task::UserInfoMysql(WFMySQLTask *mysqltask, HttpResp *resp, string &name)
{
  int state = mysqltask->get_state();
  int error = mysqltask->get_error();
  if (state != WFT_STATE_SUCCESS)
  {
    printf("%s\n", WFGlobal::get_error_string(state, error));
    return;
  }
  // 1. 检测SQL语句是否存在语法错误
  auto mysqlResp = mysqltask->get_resp();
  if (mysqlResp->get_packet_type() == MYSQL_PACKET_ERROR)
  {
    printf("ERROR %d: %s\n", mysqlResp->get_error_code(),
           mysqlResp->get_error_msg().c_str());
    return;
  }
  using namespace protocol;
  MySQLResultCursor cursor(mysqlResp);
  if (cursor.get_cursor_status() == MYSQL_STATUS_OK)
  {
    // 2. 成功写入数据库了
    printf("Query OK. %llu row affected.\n", cursor.get_affected_rows());
  }
  else if (cursor.get_cursor_status() == MYSQL_STATUS_GET_RESULT)
  {
    // 3. 读取数据
    vector<vector<MySQLCell>> matrix;
    cursor.fetch_all(matrix);
    string signupAt = matrix[0][0].as_string();
    cout << "signupAt:" << signupAt << "\n";

    using Json = nlohmann::json;
    Json msg;
    Json data;
    data["Username"] = name;
    data["SignupAt"] = signupAt;
    msg["data"] = data;
    resp->String(msg.dump());
  }
  else
  {
    resp->String("error");
  }
}

void Task::FileRequire(const HttpResp *req, HttpResp *resp,
                       SeriesWork *series) {}

void Task::FileRequireMysql(WFMySQLTask *mysqltask) {}

void Task::FileUpload(const HttpReq *req, HttpResp *resp, SeriesWork *series)
{
  string username = req->query("username");
  string tokenStr = req->query("token");
  cout << "username:" << username << endl;
  cout << "token:" << tokenStr << endl;
  // 2. 对token进行验证
  string redisurl("redis://127.0.0.1:6379");
  auto redisTask = WFTaskFactory::create_redis_task(redisurl, 0, bind(&Task::FileUploadRedis, this, std::placeholders::_1, req, resp, tokenStr));

  redisTask->get_req()
      ->set_request("HGET", {"Token", username});
  series->push_back(redisTask);
  // // 3. 解析请求：消息体
  // if (req->content_type() == MULTIPART_FORM_DATA)
  // {
  //   auto form = req->form();
  //   string filename = form["file"].first;
  //   string content = form["file"].second;
  //   // 4. 将数据写入服务器本地
  //   mkdir("tmp", 0755);
  //   string filepath = "tmp/" + filename;
  //   cout << "filepath:" << filepath << endl;
  //   int fd = open(filepath.c_str(), O_CREAT | O_RDWR, 0664);
  //   if (fd < 0)
  //   {
  //     perror("open");
  //     return;
  //   }
  //   write(fd, content.c_str(), content.size());
  //   close(fd);
  //   resp->String("upload Success");

  //   // 5. 生成SHA1值
  //   Hash hash(filepath);
  //   string filehash = hash.sha1();
  //   cout << "filehash:" << filehash << endl;
  //   // 6.将文件相关信息写入数据库MySQL中
  //   string mysqlurl("mysql://root:123@localhost");
  //   auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 1, nullptr);
  //   string sql("INSERT INTO echocloud.tbl_user_file(user_name,file_sha1,file_size,file_name)VALUES('");
  //   sql += username + "','" + filehash + "', " + std::to_string(content.size()) + ",'" + filename + "')";
  //   cout << "\nsql:\n"
  //        << sql << endl;
  //   mysqlTask->get_req()->set_query(sql);
  //   series->push_back(mysqlTask);
  // }
}

void Task::FileUploadRedis(WFRedisTask *redistask, const HttpReq *req, HttpResp *resp, string &tokenStr)
{
  // Step 1: Check the Redis task state
  if (redistask->get_state() != WFT_STATE_SUCCESS)
  {
    resp->set_status_code("500 Internal Server Error");
    resp->append_output_body("<html><body><h1>500 Internal Server Error</h1><p>Token validation failed due to Redis task error.</p></body></html>");
    return;
  }

  protocol::RedisResponse *redis_resp = redistask->get_resp();
  protocol::RedisValue val;
  redis_resp->get_result(val);
  // Step 2: Parse the response from Redis
  if (val.is_nil())
  {

    resp->set_status_code("401 Unauthorized");
    resp->append_output_body("<html><body><h1>401 Unauthorized</h1><p>Invalid or missing token.</p></body></html>");
    return;
  }
  // If the token is valid, you would continue with the file upload process.
  // For example, you may want to proceed with file saving, database update, etc.
  // Since these steps would depend on the remaining part of your workflow,
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
        // 4. 将数据写入服务器本地
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
        string sql("INSERT INTO echocloud.tbl_user_file(user_name,file_sha1,file_size,file_name)VALUES('");
        sql += username + "','" + filehash + "', " + std::to_string(content.size()) + ",'" + filename + "')";
        cout << "\nsql:\n"
             << sql << endl;
        mysqlTask->get_req()->set_query(sql);
        series_of(redistask)->push_back(mysqlTask);
      }
    }
  }
  // resp->set_status_code("200 OK");
  // resp->append_output_body("<html><body><h1>200 OK</h1><p>Token validated successfully.</p></body></html>");
}
void Task::FileDownLoad(const HttpReq *req, HttpResp *resp)
{
  string filename = req->query("filename");
  cout << "filename: " << filename << endl;

  // 将下载业务从服务器中分离出去，之后只需要产生一个下载链接就可以了
  // 这要求我们还需要去部署一个下载服务器
  string downloadURL = "http://192.168.190.128:8080/" + filename;
  resp->String(downloadURL);
}
string Task::generate_salt()
{
  const std::string salt_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
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

string Task::hash_password(const std::string &password, const std::string &salt)
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