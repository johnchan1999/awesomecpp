#pragma once
#include <stdio.h>
#include <string>
#include "srpc/rpc_define.h"
#include "../protocpp/userinfo.pb.h"

namespace userinfo
{

	namespace UserInfoService
	{

		/*
		 * Server codes
		 * Generated by SRPC
		 */

		class Service : public srpc::RPCService
		{
		public:
			// please implement these methods in server.cc

			virtual void UserInfo(ReqUserinfo *request, RespUserinfo *response,
								  srpc::RPCContext *ctx) = 0;

		public:
			Service();
		};

		/*
		 * Client codes
		 * Generated by SRPC
		 */

		using UserInfoDone = std::function<void(RespUserinfo *, srpc::RPCContext *)>;

		class SRPCClient : public srpc::SRPCClient
		{
		public:
			void UserInfo(const ReqUserinfo *req, UserInfoDone done);
			void UserInfo(const ReqUserinfo *req, SeriesWork *series, UserInfoDone done);
			void UserInfo(const ReqUserinfo *req, RespUserinfo *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<RespUserinfo, srpc::RPCSyncContext>> async_UserInfo(const ReqUserinfo *req);

		public:
			SRPCClient(const char *host, unsigned short port);
			SRPCClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::SRPCClientTask *create_UserInfo_task(UserInfoDone done);
		};

		class SRPCHttpClient : public srpc::SRPCHttpClient
		{
		public:
			void UserInfo(const ReqUserinfo *req, UserInfoDone done);
			void UserInfo(const ReqUserinfo *req, RespUserinfo *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<RespUserinfo, srpc::RPCSyncContext>> async_UserInfo(const ReqUserinfo *req);

		public:
			SRPCHttpClient(const char *host, unsigned short port);
			SRPCHttpClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::SRPCHttpClientTask *create_UserInfo_task(UserInfoDone done);
		};

		class BRPCClient : public srpc::BRPCClient
		{
		public:
			void UserInfo(const ReqUserinfo *req, UserInfoDone done);
			void UserInfo(const ReqUserinfo *req, RespUserinfo *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<RespUserinfo, srpc::RPCSyncContext>> async_UserInfo(const ReqUserinfo *req);

		public:
			BRPCClient(const char *host, unsigned short port);
			BRPCClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::BRPCClientTask *create_UserInfo_task(UserInfoDone done);
		};

		class TRPCClient : public srpc::TRPCClient
		{
		public:
			void UserInfo(const ReqUserinfo *req, UserInfoDone done);
			void UserInfo(const ReqUserinfo *req, RespUserinfo *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<RespUserinfo, srpc::RPCSyncContext>> async_UserInfo(const ReqUserinfo *req);

		public:
			TRPCClient(const char *host, unsigned short port);
			TRPCClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::TRPCClientTask *create_UserInfo_task(UserInfoDone done);
		};

		class TRPCHttpClient : public srpc::TRPCHttpClient
		{
		public:
			void UserInfo(const ReqUserinfo *req, UserInfoDone done);
			void UserInfo(const ReqUserinfo *req, RespUserinfo *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<RespUserinfo, srpc::RPCSyncContext>> async_UserInfo(const ReqUserinfo *req);

		public:
			TRPCHttpClient(const char *host, unsigned short port);
			TRPCHttpClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::TRPCHttpClientTask *create_UserInfo_task(UserInfoDone done);
		};

		///// implements detials /////

		inline Service::Service() : srpc::RPCService("userinfo.UserInfoService")
		{
			this->srpc::RPCService::add_method("UserInfo",
											   [this](srpc::RPCWorker &worker) -> int
											   {
												   return ServiceRPCCallImpl(this, worker, &Service::UserInfo);
											   });
		}

		inline SRPCClient::SRPCClient(const char *host, unsigned short port) : srpc::SRPCClient("userinfo.UserInfoService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::SRPCClient::init(&params);
		}

		inline SRPCClient::SRPCClient(const struct srpc::RPCClientParams *params) : srpc::SRPCClient("userinfo.UserInfoService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::SRPCClient::init(temp);
		}

		inline void SRPCClient::UserInfo(const ReqUserinfo *req, UserInfoDone done)
		{
			auto *task = this->create_rpc_client_task("UserInfo", std::move(done));

			task->serialize_input(req);
			task->start();
		}
		inline void SRPCClient::UserInfo(const ReqUserinfo *req, SeriesWork *series, UserInfoDone done)
		{
			auto *task = this->create_rpc_client_task("UserInfo", std::move(done));

			task->serialize_input(req);
			series->push_back(task);
		}

		inline void SRPCClient::UserInfo(const ReqUserinfo *req, RespUserinfo *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_UserInfo(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<RespUserinfo, srpc::RPCSyncContext>> SRPCClient::async_UserInfo(const ReqUserinfo *req)
		{
			using RESULT = std::pair<RespUserinfo, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<RespUserinfo>("UserInfo", srpc::RPCAsyncFutureCallback<RespUserinfo>);

			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::SRPCClientTask *SRPCClient::create_UserInfo_task(UserInfoDone done)
		{
			return this->create_rpc_client_task("UserInfo", std::move(done));
		}

		inline SRPCHttpClient::SRPCHttpClient(const char *host, unsigned short port) : srpc::SRPCHttpClient("userinfo/UserInfoService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::SRPCHttpClient::init(&params);
		}

		inline SRPCHttpClient::SRPCHttpClient(const struct srpc::RPCClientParams *params) : srpc::SRPCHttpClient("userinfo/UserInfoService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::SRPCHttpClient::init(temp);
		}

		inline void SRPCHttpClient::UserInfo(const ReqUserinfo *req, UserInfoDone done)
		{
			auto *task = this->create_rpc_client_task("UserInfo", std::move(done));

			task->serialize_input(req);
			task->start();
		}

		inline void SRPCHttpClient::UserInfo(const ReqUserinfo *req, RespUserinfo *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_UserInfo(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<RespUserinfo, srpc::RPCSyncContext>> SRPCHttpClient::async_UserInfo(const ReqUserinfo *req)
		{
			using RESULT = std::pair<RespUserinfo, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<RespUserinfo>("UserInfo", srpc::RPCAsyncFutureCallback<RespUserinfo>);

			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::SRPCHttpClientTask *SRPCHttpClient::create_UserInfo_task(UserInfoDone done)
		{
			return this->create_rpc_client_task("UserInfo", std::move(done));
		}

		inline BRPCClient::BRPCClient(const char *host, unsigned short port) : srpc::BRPCClient("userinfo.UserInfoService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::BRPCClient::init(&params);
		}

		inline BRPCClient::BRPCClient(const struct srpc::RPCClientParams *params) : srpc::BRPCClient("userinfo.UserInfoService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::BRPCClient::init(temp);
		}

		inline void BRPCClient::UserInfo(const ReqUserinfo *req, UserInfoDone done)
		{
			auto *task = this->create_rpc_client_task("UserInfo", std::move(done));

			task->serialize_input(req);
			task->start();
		}

		inline void BRPCClient::UserInfo(const ReqUserinfo *req, RespUserinfo *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_UserInfo(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<RespUserinfo, srpc::RPCSyncContext>> BRPCClient::async_UserInfo(const ReqUserinfo *req)
		{
			using RESULT = std::pair<RespUserinfo, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<RespUserinfo>("UserInfo", srpc::RPCAsyncFutureCallback<RespUserinfo>);

			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::BRPCClientTask *BRPCClient::create_UserInfo_task(UserInfoDone done)
		{
			return this->create_rpc_client_task("UserInfo", std::move(done));
		}

		inline TRPCClient::TRPCClient(const char *host, unsigned short port) : srpc::TRPCClient("userinfo.UserInfoService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::TRPCClient::init(&params);
		}

		inline TRPCClient::TRPCClient(const struct srpc::RPCClientParams *params) : srpc::TRPCClient("userinfo.UserInfoService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::TRPCClient::init(temp);
		}

		inline void TRPCClient::UserInfo(const ReqUserinfo *req, UserInfoDone done)
		{
			auto *task = this->create_rpc_client_task("/userinfo.UserInfoService/UserInfo", std::move(done));

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->start();
		}

		inline void TRPCClient::UserInfo(const ReqUserinfo *req, RespUserinfo *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_UserInfo(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<RespUserinfo, srpc::RPCSyncContext>> TRPCClient::async_UserInfo(const ReqUserinfo *req)
		{
			using RESULT = std::pair<RespUserinfo, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<RespUserinfo>("/userinfo.UserInfoService/UserInfo", srpc::RPCAsyncFutureCallback<RespUserinfo>);

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::TRPCClientTask *TRPCClient::create_UserInfo_task(UserInfoDone done)
		{
			auto *task = this->create_rpc_client_task("/userinfo.UserInfoService/UserInfo", std::move(done));

			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);

			return task;
		}

		inline TRPCHttpClient::TRPCHttpClient(const char *host, unsigned short port) : srpc::TRPCHttpClient("userinfo.UserInfoService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::TRPCHttpClient::init(&params);
		}

		inline TRPCHttpClient::TRPCHttpClient(const struct srpc::RPCClientParams *params) : srpc::TRPCHttpClient("userinfo.UserInfoService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::TRPCHttpClient::init(temp);
		}

		inline void TRPCHttpClient::UserInfo(const ReqUserinfo *req, UserInfoDone done)
		{
			auto *task = this->create_rpc_client_task("UserInfo", std::move(done));

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->start();
		}

		inline void TRPCHttpClient::UserInfo(const ReqUserinfo *req, RespUserinfo *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_UserInfo(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<RespUserinfo, srpc::RPCSyncContext>> TRPCHttpClient::async_UserInfo(const ReqUserinfo *req)
		{
			using RESULT = std::pair<RespUserinfo, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<RespUserinfo>("UserInfo", srpc::RPCAsyncFutureCallback<RespUserinfo>);

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::TRPCHttpClientTask *TRPCHttpClient::create_UserInfo_task(UserInfoDone done)
		{
			auto *task = this->create_rpc_client_task("UserInfo", std::move(done));

			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);

			return task;
		}

	} // end namespace UserInfoService

} // end namespace userinfo
