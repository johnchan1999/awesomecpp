#pragma once
#include <stdio.h>
#include <string>
#include "srpc/rpc_define.h"
#include "../protocpp/filequery.pb.h"

namespace filequery
{

	namespace filequeryService
	{

		/*
		 * Server codes
		 * Generated by SRPC
		 */

		class Service : public srpc::RPCService
		{
		public:
			// please implement these methods in server.cc

			virtual void Filequery(ReqFilequery *request, RespFilequery *response,
								   srpc::RPCContext *ctx) = 0;

		public:
			Service();
		};

		/*
		 * Client codes
		 * Generated by SRPC
		 */

		using FilequeryDone = std::function<void(RespFilequery *, srpc::RPCContext *)>;

		class SRPCClient : public srpc::SRPCClient
		{
		public:
			void Filequery(const ReqFilequery *req, FilequeryDone done);
			void Filequery(const ReqFilequery *req, SeriesWork *series, FilequeryDone done);
			void Filequery(const ReqFilequery *req, RespFilequery *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<RespFilequery, srpc::RPCSyncContext>> async_Filequery(const ReqFilequery *req);

		public:
			SRPCClient(const char *host, unsigned short port);
			SRPCClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::SRPCClientTask *create_Filequery_task(FilequeryDone done);
		};

		class SRPCHttpClient : public srpc::SRPCHttpClient
		{
		public:
			void Filequery(const ReqFilequery *req, FilequeryDone done);
			void Filequery(const ReqFilequery *req, RespFilequery *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<RespFilequery, srpc::RPCSyncContext>> async_Filequery(const ReqFilequery *req);

		public:
			SRPCHttpClient(const char *host, unsigned short port);
			SRPCHttpClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::SRPCHttpClientTask *create_Filequery_task(FilequeryDone done);
		};

		class BRPCClient : public srpc::BRPCClient
		{
		public:
			void Filequery(const ReqFilequery *req, FilequeryDone done);
			void Filequery(const ReqFilequery *req, RespFilequery *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<RespFilequery, srpc::RPCSyncContext>> async_Filequery(const ReqFilequery *req);

		public:
			BRPCClient(const char *host, unsigned short port);
			BRPCClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::BRPCClientTask *create_Filequery_task(FilequeryDone done);
		};

		class TRPCClient : public srpc::TRPCClient
		{
		public:
			void Filequery(const ReqFilequery *req, FilequeryDone done);
			void Filequery(const ReqFilequery *req, RespFilequery *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<RespFilequery, srpc::RPCSyncContext>> async_Filequery(const ReqFilequery *req);

		public:
			TRPCClient(const char *host, unsigned short port);
			TRPCClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::TRPCClientTask *create_Filequery_task(FilequeryDone done);
		};

		class TRPCHttpClient : public srpc::TRPCHttpClient
		{
		public:
			void Filequery(const ReqFilequery *req, FilequeryDone done);
			void Filequery(const ReqFilequery *req, RespFilequery *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<RespFilequery, srpc::RPCSyncContext>> async_Filequery(const ReqFilequery *req);

		public:
			TRPCHttpClient(const char *host, unsigned short port);
			TRPCHttpClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::TRPCHttpClientTask *create_Filequery_task(FilequeryDone done);
		};

		///// implements detials /////

		inline Service::Service() : srpc::RPCService("filequery.filequeryService")
		{
			this->srpc::RPCService::add_method("Filequery",
											   [this](srpc::RPCWorker &worker) -> int
											   {
												   return ServiceRPCCallImpl(this, worker, &Service::Filequery);
											   });
		}

		inline SRPCClient::SRPCClient(const char *host, unsigned short port) : srpc::SRPCClient("filequery.filequeryService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::SRPCClient::init(&params);
		}

		inline SRPCClient::SRPCClient(const struct srpc::RPCClientParams *params) : srpc::SRPCClient("filequery.filequeryService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::SRPCClient::init(temp);
		}

		inline void SRPCClient::Filequery(const ReqFilequery *req, FilequeryDone done)
		{
			auto *task = this->create_rpc_client_task("Filequery", std::move(done));

			task->serialize_input(req);
			task->start();
		}

		inline void SRPCClient::Filequery(const ReqFilequery *req, SeriesWork *series, FilequeryDone done)
		{
			auto *task = this->create_rpc_client_task("Filequery", std::move(done));

			task->serialize_input(req);
			series->push_back(task);
		}
		inline void SRPCClient::Filequery(const ReqFilequery *req, RespFilequery *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_Filequery(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<RespFilequery, srpc::RPCSyncContext>> SRPCClient::async_Filequery(const ReqFilequery *req)
		{
			using RESULT = std::pair<RespFilequery, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<RespFilequery>("Filequery", srpc::RPCAsyncFutureCallback<RespFilequery>);

			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::SRPCClientTask *SRPCClient::create_Filequery_task(FilequeryDone done)
		{
			return this->create_rpc_client_task("Filequery", std::move(done));
		}

		inline SRPCHttpClient::SRPCHttpClient(const char *host, unsigned short port) : srpc::SRPCHttpClient("filequery/filequeryService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::SRPCHttpClient::init(&params);
		}

		inline SRPCHttpClient::SRPCHttpClient(const struct srpc::RPCClientParams *params) : srpc::SRPCHttpClient("filequery/filequeryService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::SRPCHttpClient::init(temp);
		}

		inline void SRPCHttpClient::Filequery(const ReqFilequery *req, FilequeryDone done)
		{
			auto *task = this->create_rpc_client_task("Filequery", std::move(done));

			task->serialize_input(req);
			task->start();
		}

		inline void SRPCHttpClient::Filequery(const ReqFilequery *req, RespFilequery *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_Filequery(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<RespFilequery, srpc::RPCSyncContext>> SRPCHttpClient::async_Filequery(const ReqFilequery *req)
		{
			using RESULT = std::pair<RespFilequery, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<RespFilequery>("Filequery", srpc::RPCAsyncFutureCallback<RespFilequery>);

			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::SRPCHttpClientTask *SRPCHttpClient::create_Filequery_task(FilequeryDone done)
		{
			return this->create_rpc_client_task("Filequery", std::move(done));
		}

		inline BRPCClient::BRPCClient(const char *host, unsigned short port) : srpc::BRPCClient("filequery.filequeryService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::BRPCClient::init(&params);
		}

		inline BRPCClient::BRPCClient(const struct srpc::RPCClientParams *params) : srpc::BRPCClient("filequery.filequeryService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::BRPCClient::init(temp);
		}

		inline void BRPCClient::Filequery(const ReqFilequery *req, FilequeryDone done)
		{
			auto *task = this->create_rpc_client_task("Filequery", std::move(done));

			task->serialize_input(req);
			task->start();
		}

		inline void BRPCClient::Filequery(const ReqFilequery *req, RespFilequery *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_Filequery(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<RespFilequery, srpc::RPCSyncContext>> BRPCClient::async_Filequery(const ReqFilequery *req)
		{
			using RESULT = std::pair<RespFilequery, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<RespFilequery>("Filequery", srpc::RPCAsyncFutureCallback<RespFilequery>);

			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::BRPCClientTask *BRPCClient::create_Filequery_task(FilequeryDone done)
		{
			return this->create_rpc_client_task("Filequery", std::move(done));
		}

		inline TRPCClient::TRPCClient(const char *host, unsigned short port) : srpc::TRPCClient("filequery.filequeryService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::TRPCClient::init(&params);
		}

		inline TRPCClient::TRPCClient(const struct srpc::RPCClientParams *params) : srpc::TRPCClient("filequery.filequeryService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::TRPCClient::init(temp);
		}

		inline void TRPCClient::Filequery(const ReqFilequery *req, FilequeryDone done)
		{
			auto *task = this->create_rpc_client_task("/filequery.filequeryService/Filequery", std::move(done));

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->start();
		}

		inline void TRPCClient::Filequery(const ReqFilequery *req, RespFilequery *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_Filequery(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<RespFilequery, srpc::RPCSyncContext>> TRPCClient::async_Filequery(const ReqFilequery *req)
		{
			using RESULT = std::pair<RespFilequery, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<RespFilequery>("/filequery.filequeryService/Filequery", srpc::RPCAsyncFutureCallback<RespFilequery>);

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::TRPCClientTask *TRPCClient::create_Filequery_task(FilequeryDone done)
		{
			auto *task = this->create_rpc_client_task("/filequery.filequeryService/Filequery", std::move(done));

			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);

			return task;
		}

		inline TRPCHttpClient::TRPCHttpClient(const char *host, unsigned short port) : srpc::TRPCHttpClient("filequery.filequeryService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::TRPCHttpClient::init(&params);
		}

		inline TRPCHttpClient::TRPCHttpClient(const struct srpc::RPCClientParams *params) : srpc::TRPCHttpClient("filequery.filequeryService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::TRPCHttpClient::init(temp);
		}

		inline void TRPCHttpClient::Filequery(const ReqFilequery *req, FilequeryDone done)
		{
			auto *task = this->create_rpc_client_task("Filequery", std::move(done));

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->start();
		}

		inline void TRPCHttpClient::Filequery(const ReqFilequery *req, RespFilequery *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_Filequery(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<RespFilequery, srpc::RPCSyncContext>> TRPCHttpClient::async_Filequery(const ReqFilequery *req)
		{
			using RESULT = std::pair<RespFilequery, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<RespFilequery>("Filequery", srpc::RPCAsyncFutureCallback<RespFilequery>);

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::TRPCHttpClientTask *TRPCHttpClient::create_Filequery_task(FilequeryDone done)
		{
			auto *task = this->create_rpc_client_task("Filequery", std::move(done));

			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);

			return task;
		}

	} // end namespace filequeryService

} // end namespace filequery