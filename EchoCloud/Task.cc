#include "Task.h"
#include <iostream>
#include <random>
#include <vector>
#include <workflow/MySQLMessage.h>
#include <workflow/MySQLResult.h>
using std::cout;
using std::vector;

Task::Task() {}
Task::~Task() {}

void Task::StaticResourceSignUp(const HttpReq *, HttpResp *resp) {
  resp->File("static/view/signup.html");
}

void Task::StaticResourceSignIn(const HttpReq *, HttpResp *resp) {
  resp->File("static/view/signin.html");
}

void Task::StaticResourceHome(const HttpReq *, HttpResp *resp) {
  resp->File("static/view/home.html");
}

void Task::StaticResourceJs(const HttpReq *, HttpResp *resp) {
  resp->File("static/js/auth.js");
}

void Task::StaticResourceJpeg(const HttpReq *, HttpResp *resp) {
  resp->File("static/img/avatar.jpeg");
}

void Task::StaticResourceUpload(const HttpReq *, HttpResp *resp) {
  resp->File("static/view/index.html");
}

// void Task::StaticResourceUploadFile(const HttpReq *, HttpResp *resp) {}

void Task::RegisterSignIn(const HttpReq *req, HttpResp *resp,
                          SeriesWork *series) {
  if (req->content_type() == wfrest::APPLICATION_URLENCODED) {
    auto formkv = req->form_kv();
    string name = formkv["username"];
    string password = formkv["password"];
    cout << "name =" << name << ",password =" << password << "\n";
    string salt_value;
    salt_value = salt(password);
    string encodePassword(crypt(password.c_str(), salt_value.c_str()));

    cout << "mi wen :" << encodePassword << "\n";

    string mysqlurl("mysql://root:cj19991114@localhost");
    auto mysqlTask = WFTaskFactory::create_mysql_task(
        mysqlurl, 1,
        std::bind(&Task::RegisterSignInMysql, this, std::placeholders::_1, resp,
                  name, encodePassword, series));
    string sql("INSERT INTO echocloud.tbl_user(user_name,user_pwd) VALUE ('");
    sql += name + "','" + encodePassword + "')";
    cout << "sql:\n" << sql << "\n";
    mysqlTask->get_req()->set_query(sql);
    series->push_back(mysqlTask);
  }
}

void Task::RegisterSignInMysql(WFMySQLTask *mysqltask, HttpResp *resp,
                               string name, string encodePassword,
                               SeriesWork *series) {
  int state = mysqltask->get_state();
  int error = mysqltask->get_error();
  if (state != WFT_STATE_SUCCESS) {
    cout << WFGlobal::get_error_string(state, error) << "\n";
    return;
  }

  auto mysqlResp = mysqltask->get_resp();
  if (mysqlResp->get_packet_type() == MYSQL_PACKET_ERROR) {
    cout << "ERROR" << mysqlResp->get_error_code() << ":"
         << mysqlResp->get_error_msg() << "\n";
    resp->String("Signup failed");
    return;
  }

  using namespace ::protocol;
  MySQLResultCursor cursor(mysqlResp);
  if (cursor.get_cursor_status() == MYSQL_STATUS_OK) {
    cout << "Query OK." << cursor.get_affected_rows() << "row affected \n";
    resp->String("SUCCESS!");
    string redisurl = "redis://127.0.0.1:6379";
    WFRedisTask *redisTask = WFTaskFactory::create_redis_task(
        redisurl, 0,
        bind(&Task::RegisterSignInRedis, this, std::placeholders::_1));
    protocol::RedisRequest *req = redisTask->get_req();
    req->set_request("HSET", {"user", name, encodePassword});
    series->push_back(redisTask);
  } else {
    resp->String("SignUp failed!");
  }
}
// void Task::RegisterSignInRedis(WFRedisTask *redistask) {
//   protocol::RedisResponse *resp = redistask->get_resp();
//   protocol::RedisValue value;
//   resp->get_result(value);
// }
void Task::RegisterSignInRedis(WFRedisTask *redistask) {
  protocol::RedisResponse *resp = redistask->get_resp();
  protocol::RedisValue value;

  // 获取结果
  int error = redistask->get_state();
  resp->get_result(value);

  // 检查任务状态以及响应结果
  if (error == WFT_STATE_SUCCESS && value.is_int()) {
    long long result = value.int_value();
    cout << result << "\n";
    if (result == 1) {
      // HSET 成功，并且字段是新的
      std::cout << "HSET successful with a new field set." << std::endl;
    } else if (result == 0) {
      // HSET 成功，但字段已存在，旧值被新值覆盖
      std::cout << "HSET successful with an existing field overwritten."
                << std::endl;
    } else {
      // 这通常不应该发生，因为 HSET 应该返回 0 或 1
      std::cerr << "HSET returned an unexpected result." << std::endl;
    }
  } else {
    // 操作失败或返回结果类型不是整数
    std::cerr << "HSET operation failed or return type is not integer."
              << std::endl;
  }
}

void Task::LoginSignIn(const HttpReq *req, HttpResp *resp, SeriesWork *series) {
  if (req->content_type() == APPLICATION_URLENCODED) {
    // 解析请求获取用户名和密码
    auto formkv = req->form_kv();
    string name = formkv["username"];
    string password = formkv["password"];
    cout << "username =" << name << ", password =" << password << "\n";

    string salt_value;
    salt_value = salt(password);
    string encodePassword(crypt(password.c_str(), salt_value.c_str()));
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
                            string encodePassword, string salt_value) {

  protocol::RedisRequest *req = redisTask->get_req();
  protocol::RedisResponse *redisresp = redisTask->get_resp();
  protocol::RedisValue value; // 将要用来获取redis返回的结果
  int state = redisTask->get_state();
  int error = redisTask->get_error(); // error只能处理网络连接和系统错误
  switch (state) {
  case WFT_STATE_SYS_ERROR:
    cout << "system error:" << strerror(error) << "\n";
    break;
  case WFT_STATE_DNS_ERROR:
    cout << "dns error: " << gai_strerror(error) << "\n";
    break;
  case WFT_STATE_SUCCESS:
    redisresp->get_result(value); // 将redis的结果已经存在value中了
    if (value.is_error()) {
      cout << "redis error\n";
      state = WFT_STATE_TASK_ERROR; // 任务层面的错误
    }
    break;
  }

  if (state == WFT_STATE_SUCCESS) {
    if (value.is_nil()) {
      cout << "User not found in Redis, checking MySQL..." << "\n";

      string mysqlurl("mysql//:root:cj19991114@localhost");
      auto *mysqlTask = WFTaskFactory::create_mysql_task(
          mysqlurl, 1,
          std::bind(&Task::LoginSignInMysql, this, std::placeholders::_1, resp,
                    name, encodePassword, salt_value));

      string sql("select user_pwd from cloudisk.tbl_user where user_name = '");
      sql += name + "' limit 1";
      cout << "sql:\n" << sql << "\n";
      mysqlTask->get_req()->set_query(sql);
      series_of(redisTask)->push_back(mysqlTask);

    } else if (value.is_string()) {
      cout << "Found user in Redis, value = " << value.string_value() << "\n";
      if (value.string_value() == encodePassword) {
        resp->String("Login SUCESS");
      }
    }
    // ... 其他情况的处理 ...
  } else {
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
                            string encodePassword, string salt_value) {
  int state = mysqltask->get_state();
  int error = mysqltask->get_error();
  if (state != WFT_STATE_SUCCESS) {
    printf("%s\n", WFGlobal::get_error_string(state, error));
    return;
  }
  // 1. 检测SQL语句是否存在语法错误
  auto mysqlResp = mysqltask->get_resp();
  if (mysqlResp->get_packet_type() == MYSQL_PACKET_ERROR) {
    printf("ERROR %d: %s\n", mysqlResp->get_error_code(),
           mysqlResp->get_error_msg().c_str());
    resp->String("Singup Failed");
    return;
  }
  using namespace protocol;
  MySQLResultCursor cursor(mysqlResp);
  if (cursor.get_cursor_status() == MYSQL_STATUS_OK) {
    // 2. 成功写入数据库了
    printf("Query OK. %llu row affected.\n", cursor.get_affected_rows());
    resp->String("Login Failed");
  } else if (cursor.get_cursor_status() == MYSQL_STATUS_GET_RESULT) {
    // 3. 读取数据
    vector<vector<MySQLCell>> matrix;
    cursor.fetch_all(matrix);
    string M = matrix[0][0].as_string();
    cout << "M:" << M << "\n";
    if (encodePassword == M) {
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
      // // 3.3 将Token保存到数据库中
      // string mysqlurl("mysql://root:cj19991114@localhost");
      // auto nextTask = WFTaskFactory::create_mysql_task(mysqlurl, 1, nullptr);
      // string sql("REPLACE INTO cloudisk.tbl_user_token(user_name, "
      //            "user_token)VALUES('");
      // sql += name + "', '" + tokenStr + "')";
      // nextTask->get_req()->set_query(sql);

    } else {
      // 登录失败的情况
      resp->String("Login Failed");
    }
  }
}

// void Task::LoginSignInMysqlToken(const HttpReq * req,HttpResp * resp,)

void Task::UserInfo(const HttpReq *req, HttpResp *resp, SeriesWork *series) {}

void Task::UserInfoMysql(WFMySQLTask *mysqltask) {}

void Task::FileRequire(const HttpResp *req, HttpResp *resp,
                       SeriesWork *series) {}

void Task::FileRequireMysql(WFMySQLTask *mysqltask) {}

void Task::FileUpload(const HttpReq *req, HttpResp *resp, SeriesWork *series) {}

string Task::salt(string &password) {
  std::random_device rd;
  std::mt19937 generator(rd());

  std::uniform_int_distribution<> distribution('!', '~');

  string salt;

  std::generate_n(std::back_inserter(salt), 26,
                  [&]() { return static_cast<char>(distribution(generator)); });
  return salt;
}
