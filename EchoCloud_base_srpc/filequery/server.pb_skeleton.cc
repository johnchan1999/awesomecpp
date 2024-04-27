#include "filequery.srpc.h"
#include "workflow/WFFacilities.h"

#include <string>
#include <iostream>
#include <wfrest/MysqlUtil.h>

#include <workflow/WFFacilities.h>
#include <workflow/MySQLUtil.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLMessage.h>
#include <ppconsul/ppconsul.h>

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

class filequeryServiceServiceImpl : public ::filequery::filequeryService::Service
{
public:
	void Filequery(::filequery::ReqFilequery *request, ::filequery::RespFilequery *response, srpc::RPCContext *ctx) override
	{
		// TODO: fill server logic here
		string limitCnt = request->extr_message();
		string username = request->username();
		string token = request->tokenstr();
		string mysqlurl("mysql://root:cj19991114@localhost");
		auto mysqlTask = WFTaskFactory::create_mysql_task(mysqlurl, 2, std::bind(&filequeryServiceServiceImpl::FileRequireMysql, this, std::placeholders::_1, response));

		string sql("select file_sha1, file_name, file_size, upload_at, last_update from echocloud.tbl_user_file where user_name = '");
		sql += username + "' limit " + limitCnt;
		cout << "\nsql:\n"
			 << sql << "\n";
		mysqlTask->get_req()->set_query(sql);
		ctx->get_series()->push_back(mysqlTask);
	}
	void FileRequireMysql(WFMySQLTask *mysqltask, ::filequery::RespFilequery *response)
	{
		using namespace protocol;
		auto mysqlResp = mysqltask->get_resp();
		MySQLResultCursor cursor(mysqlResp);
		if (cursor.get_cursor_status() == MYSQL_STATUS_GET_RESULT)
		{
			// 读操作,获取用户的
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
				msgArr.push_back(row); // 在数组中添加一个元素,使用push_back即可
			}
			response->set_code(0);
			response->set_message("success");
			response->set_extr_message(msgArr.dump());
		}
		else
		{
			// 没有读取到正确的信息
			response->set_code(1);
			response->set_message("error");
		}
	}
};

int main()
{

	GOOGLE_PROTOBUF_VERIFY_VERSION;
	unsigned short port = 10030;
	SRPCServer server;

	filequeryServiceServiceImpl filequeryservice_impl;
	server.add_service(&filequeryservice_impl);

	Consul consul("1.13.79.29:8500", ppconsul::kw::dc = "dc1");
	agent::Agent agent(consul);
	agent.registerService(
		agent::kw::name = "FilequeryService1",
		agent::kw::address = "1.13.79.29",
		agent::kw::id = "FilequeryService1",
		agent::kw::port = 10030,
		agent::kw::check = agent::TtlCheck(std::chrono::seconds(10)));
	agent.servicePass("FilequeryService1");
	WFTimerTask *timerTask = WFTaskFactory::create_timer_task(9, 0, timerCallback);
	timerTask->user_data = &agent;
	timerTask->start();

	server.start(port);
	wait_group.wait();
	server.stop();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}
