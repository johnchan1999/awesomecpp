#include "signup.srpc.h"
#include "workflow/WFFacilities.h"

#include <string>
#include <iostream>
#include <wfrest/MysqlUtil.h>

#include <workflow/WFFacilities.h>
#include <workflow/MySQLUtil.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLMessage.h>
#include <ppconsul/ppconsul.h>
// #include "../api/Task.h"

using namespace ppconsul;
using std::string;
using namespace srpc;
using std::cout;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
	wait_group.done();
}

void timerCallback(WFTimerTask *timerTask)
{
	// cout << "now = " << time(nullptr) << "\n";
	agent::Agent *pagent = (agent::Agent *)timerTask->user_data;
	pagent->servicePass("SignupService1");
	WFTimerTask *nextTask = WFTaskFactory::create_timer_task(9, 0, timerCallback);
	nextTask->user_data = pagent;
	series_of(timerTask)->push_back(nextTask);
}

class SignupServiceServiceImpl : public ::signup::SignupService::Service
{
public:
	void Signup(::signup::ReqSignup *request, ::signup::RespSignup *response, srpc::RPCContext *ctx) override
	{
		string username = request->username();
		string password = request->password();

		string salt_value;
		salt_value = generate_salt1(password);
		cout << "salt_value=" << salt_value << "\n";
		string encodePassword = hash_password(password, salt_value);
		cout << "mi wen :" << encodePassword << "\n";

		string mysqlurl("mysql://root:cj19991114@localhost");
		auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 0, bind(&SignupServiceServiceImpl::RegisterSignInMysql, this, std::placeholders::_1, response, username, encodePassword, ctx));
		string sql("INSERT INTO echocloud.tbl_user(user_name,user_pwd) VALUE ('");
		sql += username + "','" + encodePassword + "')";
		cout << "sql:\n"
			 << sql << "\n";
		mysqlTask->get_req()->set_query(sql);
		ctx->get_series()->push_back(mysqlTask);
	}
	void RegisterSignInMysql(WFMySQLTask *mysqltask, ::signup::RespSignup *resp,
							 string name, string encodePassword,
							 srpc::RPCContext *ctx)
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
			resp->set_code(1);
			resp->set_message("Signup failed");
			return;
		}

		using namespace ::protocol;
		MySQLResultCursor cursor(mysqlResp);
		if (cursor.get_cursor_status() == MYSQL_STATUS_OK)
		{
			cout << "Query OK." << cursor.get_affected_rows() << "row affected \n";
			resp->set_code(0);
			resp->set_message("SUCCESS");
			string redisurl = "redis://127.0.0.1:6379";
			WFRedisTask *redisTask = WFTaskFactory::create_redis_task(
				redisurl, 0,
				bind(&SignupServiceServiceImpl::RegisterSignInRedis, this, std::placeholders::_1, resp));
			protocol::RedisRequest *req = redisTask->get_req();
			req->set_request("HSET", {"user", name, encodePassword});
			ctx->get_series()->push_back(redisTask);
		}
		else
		{
			resp->set_code(1);
			resp->set_message("SignUp failed(mysql)!");
		}
	}

	void RegisterSignInRedis(WFRedisTask *redistask, ::signup::RespSignup *resp)
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
	unsigned short port = 10000;
	SRPCServer server;

	SignupServiceServiceImpl signupservice_impl;
	server.add_service(&signupservice_impl);

	Consul consul("1.13.79.29:8500", ppconsul::kw::dc = "dc1");
	agent::Agent agent(consul);
	agent.registerService(
		agent::kw::name = "SignupService1",
		agent::kw::address = "1.13.79.29",
		agent::kw::id = "SignupService1",
		agent::kw::port = 10000,
		agent::kw::check = agent::TtlCheck(std::chrono::seconds(10)));
	agent.servicePass("SignupService1");
	WFTimerTask *timerTask = WFTaskFactory::create_timer_task(9, 0, timerCallback);
	timerTask->user_data = &agent;
	timerTask->start();

	server.start(port);
	wait_group.wait();
	server.stop();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}
