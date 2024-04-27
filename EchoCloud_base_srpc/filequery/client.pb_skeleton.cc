#include "filequery.srpc.h"
#include "workflow/WFFacilities.h"

using namespace srpc;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
	wait_group.done();
}

static void filequery_done(::filequery::RespFilequery *response, srpc::RPCContext *context)
{
}

int main()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	const char *ip = "127.0.0.1";
	unsigned short port = 1412;

	::filequery::filequeryService::SRPCClient client(ip, port);

	// example for RPC method call
	::filequery::ReqFilequery filequery_req;
	//filequery_req.set_message("Hello, srpc!");
	client.Filequery(&filequery_req, filequery_done);

	wait_group.wait();
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}
