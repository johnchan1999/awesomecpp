#include "userinfo.srpc.h"
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

class UserInfoServiceServiceImpl : public ::userinfo::UserInfoService::Service
{
public:
	void UserInfo(::userinfo::ReqUserinfo *request, ::userinfo::RespUserinfo *response, srpc::RPCContext *ctx) override
	{
		string name = request->name();
		string token = request->tokenstr();
		string mysqlurl("mysql://root:cj19991114@localhost");
		auto mysqlTask = WFTaskFactory::create_mysql_task(
			mysqlurl, 1,
			bind(&UserInfoServiceServiceImpl::UserInfoMysql, this, std::placeholders::_1, response, name));
		string sql("select signup_at from echocloud.tbl_user where user_name = '");
		sql += name + "'";
		mysqlTask->get_req()->set_query(sql);
		ctx->get_series()->push_back(mysqlTask);
	}
	void UserInfoMysql(WFMySQLTask *mysqltask, ::userinfo::RespUserinfo *response, string &name)
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

			response->set_code(0);
			response->set_message("success");
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
			response->set_extr_message(msg.dump());
		}
		else
		{
			response->set_code(1);
			response->set_message("error");
		}
	}
};

int main()
{

	GOOGLE_PROTOBUF_VERIFY_VERSION;
	unsigned short port = 10020;
	SRPCServer server;

	UserInfoServiceServiceImpl userinfoservice_impl;
	server.add_service(&userinfoservice_impl);

	Consul consul("1.13.79.29:8500", ppconsul::kw::dc = "dc1");
	agent::Agent agent(consul);
	agent.registerService(
		agent::kw::name = "UserinfoService1",
		agent::kw::address = "1.13.79.29",
		agent::kw::id = "UserinfoService1",
		agent::kw::port = 10020,
		agent::kw::check = agent::TtlCheck(std::chrono::seconds(10)));
	agent.servicePass("UserinfoService1");
	WFTimerTask *timerTask = WFTaskFactory::create_timer_task(9, 0, timerCallback);
	timerTask->user_data = &agent;
	timerTask->start();

	server.start(port);
	wait_group.wait();
	server.stop();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}
