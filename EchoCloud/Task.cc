#include "Task.h"
#include <iostream>
#include <random>

using std::cout;

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
    string name = formkv["name"];
    string password = formkv["password"];
    cout << "name =" << name << ",password =" << password << "\n";
    string encodePassword;
    encodePassword = salt(password);

    cout << "mi wen :" << encodePassword << "\n";

    string mysqlurl("mysql://root:cj19991114@localhost");
    auto mysqlTask = WFTaskFactory::create_mysql_task(
        mysqlurl, 1,
        std::bind(&Task::RegisterSignInMysql, this, std::placeholders::_1,
                  resp));
  }
}

void Task::RegisterSignInMysql(WFMySQLTask *mysqltask, HttpResp *resp) {
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
}

void Task::LoginSignIn(const HttpReq *req, HttpResp *resp, SeriesWork *series) {
}

void Task::LoginSignInMysql(WFMySQLTask *mysqltask) {}

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
