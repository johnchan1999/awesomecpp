#include "login.srpc.h"
#include "workflow/WFFacilities.h"

#include <string>
#include <iostream>
#include <wfrest/MysqlUtil.h>

#include <workflow/WFFacilities.h>
#include <workflow/MySQLUtil.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLMessage.h>
#include <ppconsul/ppconsul.h>
#include "../api/Token.h"
#include <wfrest/json.hpp>
#include <vector>
#include "../api/Token.h"

using namespace ppconsul;
using std::string;
using namespace srpc;
using std::cout;
using std::vector;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
	wait_group.done();
}
void timerCallback(WFTimerTask *timerTask)
{
	// cout << "now = " << time(nullptr) << "\n";
	agent::Agent *pagent = (agent::Agent *)timerTask->user_data;
	pagent->servicePass("LoginService1");
	WFTimerTask *nextTask = WFTaskFactory::create_timer_task(9, 0, timerCallback);
	nextTask->user_data = pagent;
	series_of(timerTask)->push_back(nextTask);
}
class loginServiceServiceImpl : public ::login::loginService::Service
{
public:
	void Login(::login::ReqLogin *request, ::login::ResqLogin *response, srpc::RPCContext *ctx) override
	{
		
		string name = request->username();
		string password = request->password();
		string salt_value;
		salt_value = generate_salt1(password);
		cout << "salt_value=" << salt_value << "\n";
		string encodePassword = hash_password(password, salt_value);
		cout << "mi wen :" << encodePassword << "\n";

		// create redistask to save user information

		string redisurl = "redis://127.0.0.1:6379";
		WFRedisTask *redisTask = WFTaskFactory::create_redis_task(
			redisurl, 0,
			bind(&loginServiceServiceImpl::LoginSignInRedis, this, std::placeholders::_1, response, name,
				 encodePassword, salt_value));

		using namespace protocol;
		RedisRequest *req = redisTask->get_req();
		req->set_request("HGET", {"user", name});
		ctx->get_series()->push_back(redisTask);
	}
	void LoginSignInRedis(WFRedisTask *redisTask, ::login::ResqLogin *response, string name,
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
				cout << "User not found in Redis, checking MySQL..."
					 << "\n";

				string mysqlurl("mysql//:root:cj19991114@localhost");
				auto *mysqlTask = WFTaskFactory::create_mysql_task(
					mysqlurl, 1,
					std::bind(&loginServiceServiceImpl::LoginSignInMysql, this, std::placeholders::_1, response,
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

					response->set_code(0);
					response->set_message(tokenStr);
					response->set_username(name);

					// 将token保存到redis中
					string redisurl = "redis://127.0.0.1:6379";
					WFRedisTask *redisTask1 =
						WFTaskFactory::create_redis_task(redisurl, 0, nullptr);
					protocol::RedisRequest *req = redisTask1->get_req();
					req->set_request("HSET", {"Token", name, tokenStr});
					series_of(redisTask)->push_back(redisTask1);

					// // 3.3 将Token保存到数据库中
					// auto nextTask = WFTaskFactory::create_mysql_task(mysqlurl, 1,
					// nullptr); string sql("REPLACE INTO cloudisk.tbl_user_token(user_name,
					// user_token)VALUES('"); sql += username + "', '" + tokenStr + "')";
					// nextTask->get_req()->set_query(sql);
					// series->push_back(nextTask);
				}
				else
				{
					response->set_code(1);
					response->set_message("Login Failed");
				}
			}
		}
		else
		{
			cout << "Redis query FAILED\n";
			// // 可能需要在这里添加MySQL查询作为备份
			// auto *mysqlTask = WFTaskFactory::create_mysql_task();
			// series_of(redisTask)->push_back(mysqlTask);
		}
	}
	void LoginSignInMysql(WFMySQLTask *mysqltask, login::ResqLogin *response, string name,
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
			response->set_code(1);
			response->set_message("Login Failed");
			return;
		}
		using namespace protocol;
		MySQLResultCursor cursor(mysqlResp);
		if (cursor.get_cursor_status() == MYSQL_STATUS_OK)
		{
			// 2. 成功写入数据库了
			printf("Query OK. %llu row affected.\n", cursor.get_affected_rows());

			response->set_code(1);
			response->set_message("Login Failed");
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
				response->set_message(tokenStr);
				response->set_code(0);
				response->set_username(name);

				// 将Token保存在redis中
				string redisurl("redis://127.0.0.1:6379");
				auto nextTask = WFTaskFactory::create_redis_task(redisurl, 0, nullptr);

				nextTask->get_req()->set_request("HSET", {"Token", name, tokenStr});
				series_of(mysqltask)->push_back(nextTask);
				cout << "nexttask has done\n";
				// 将Token保存到数据库中
				string mysqlurl("mysql://root:cj19991114@localhost");
				auto nextMysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 1, nullptr);
				string sql("REPLACE INTO cloudisk.tbl_user_token(user_name, "
						   "user_token)VALUES('");
				sql += name + "', '" + tokenStr + "')";
				nextMysqlTask->get_req()->set_query(sql);
			}
			else
			{
				// 登录失败的情况
				response->set_code(1);
				response->set_message("Login Failed");
			}
		}
	}

private:
	string
	hash_password(const std::string &password, const std::string &salt)
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
	string generate_salt1(const std::string &password)
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
};

int main()
{

	GOOGLE_PROTOBUF_VERIFY_VERSION;
	unsigned short port = 10010;
	SRPCServer server;

	loginServiceServiceImpl loginservice_impl;
	server.add_service(&loginservice_impl);

	Consul consul("1.13.79.29:8500", ppconsul::kw::dc = "dc1");
	agent::Agent agent(consul);
	agent.registerService(
		agent::kw::name = "LoginService1",
		agent::kw::address = "1.13.79.29",
		agent::kw::id = "LoginService1",
		agent::kw::port = 10010,
		agent::kw::check = agent::TtlCheck(std::chrono::seconds(10)));
	agent.servicePass("LoginService1");
	WFTimerTask *timerTask = WFTaskFactory::create_timer_task(9, 0, timerCallback);
	timerTask->user_data = &agent;
	timerTask->start();

	server.start(port);
	wait_group.wait();
	server.stop();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}
