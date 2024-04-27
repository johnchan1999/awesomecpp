#pragma once
#include <stdio.h>
#include <string>
#include "srpc/rpc_define.h"
#include "../protocpp/login.pb.h"

namespace login
{

	namespace loginService
	{

		/*
		 * Server codes
		 * Generated by SRPC
		 */

		class Service : public srpc::RPCService
		{
		public:
			// please implement these methods in server.cc

			virtual void Login(ReqLogin *request, ResqLogin *response,
							   srpc::RPCContext *ctx) = 0;

		public:
			Service();
		};

		/*
		 * Client codes
		 * Generated by SRPC
		 */

		using LoginDone = std::function<void(ResqLogin *, srpc::RPCContext *)>;

		class SRPCClient : public srpc::SRPCClient
		{
		public:
			void Login(const ReqLogin *req, LoginDone done);
			void Login(const ReqLogin *req, SeriesWork *series, LoginDone done);
			void Login(const ReqLogin *req, ResqLogin *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<ResqLogin, srpc::RPCSyncContext>> async_Login(const ReqLogin *req);

		public:
			SRPCClient(const char *host, unsigned short port);
			SRPCClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::SRPCClientTask *create_Login_task(LoginDone done);
		};

		class SRPCHttpClient : public srpc::SRPCHttpClient
		{
		public:
			void Login(const ReqLogin *req, LoginDone done);
			void Login(const ReqLogin *req, ResqLogin *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<ResqLogin, srpc::RPCSyncContext>> async_Login(const ReqLogin *req);

		public:
			SRPCHttpClient(const char *host, unsigned short port);
			SRPCHttpClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::SRPCHttpClientTask *create_Login_task(LoginDone done);
		};

		class BRPCClient : public srpc::BRPCClient
		{
		public:
			void Login(const ReqLogin *req, LoginDone done);
			void Login(const ReqLogin *req, ResqLogin *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<ResqLogin, srpc::RPCSyncContext>> async_Login(const ReqLogin *req);

		public:
			BRPCClient(const char *host, unsigned short port);
			BRPCClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::BRPCClientTask *create_Login_task(LoginDone done);
		};

		class TRPCClient : public srpc::TRPCClient
		{
		public:
			void Login(const ReqLogin *req, LoginDone done);
			void Login(const ReqLogin *req, ResqLogin *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<ResqLogin, srpc::RPCSyncContext>> async_Login(const ReqLogin *req);

		public:
			TRPCClient(const char *host, unsigned short port);
			TRPCClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::TRPCClientTask *create_Login_task(LoginDone done);
		};

		class TRPCHttpClient : public srpc::TRPCHttpClient
		{
		public:
			void Login(const ReqLogin *req, LoginDone done);
			void Login(const ReqLogin *req, ResqLogin *resp, srpc::RPCSyncContext *sync_ctx);
			WFFuture<std::pair<ResqLogin, srpc::RPCSyncContext>> async_Login(const ReqLogin *req);

		public:
			TRPCHttpClient(const char *host, unsigned short port);
			TRPCHttpClient(const struct srpc::RPCClientParams *params);

		public:
			srpc::TRPCHttpClientTask *create_Login_task(LoginDone done);
		};

		///// implements detials /////

		inline Service::Service() : srpc::RPCService("login.loginService")
		{
			this->srpc::RPCService::add_method("Login",
											   [this](srpc::RPCWorker &worker) -> int
											   {
												   return ServiceRPCCallImpl(this, worker, &Service::Login);
											   });
		}

		inline SRPCClient::SRPCClient(const char *host, unsigned short port) : srpc::SRPCClient("login.loginService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::SRPCClient::init(&params);
		}

		inline SRPCClient::SRPCClient(const struct srpc::RPCClientParams *params) : srpc::SRPCClient("login.loginService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::SRPCClient::init(temp);
		}

		inline void SRPCClient::Login(const ReqLogin *req, LoginDone done)
		{
			auto *task = this->create_rpc_client_task("Login", std::move(done));

			task->serialize_input(req);
			task->start();
		}

		inline void SRPCClient::Login(const ReqLogin *req, SeriesWork *series, LoginDone done)
		{
			auto *task = this->create_rpc_client_task("Login", std::move(done));

			task->serialize_input(req);
			series->push_back(task);
		}

		inline void SRPCClient::Login(const ReqLogin *req, ResqLogin *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_Login(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<ResqLogin, srpc::RPCSyncContext>> SRPCClient::async_Login(const ReqLogin *req)
		{
			using RESULT = std::pair<ResqLogin, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<ResqLogin>("Login", srpc::RPCAsyncFutureCallback<ResqLogin>);

			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::SRPCClientTask *SRPCClient::create_Login_task(LoginDone done)
		{
			return this->create_rpc_client_task("Login", std::move(done));
		}

		inline SRPCHttpClient::SRPCHttpClient(const char *host, unsigned short port) : srpc::SRPCHttpClient("login/loginService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::SRPCHttpClient::init(&params);
		}

		inline SRPCHttpClient::SRPCHttpClient(const struct srpc::RPCClientParams *params) : srpc::SRPCHttpClient("login/loginService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::SRPCHttpClient::init(temp);
		}

		inline void SRPCHttpClient::Login(const ReqLogin *req, LoginDone done)
		{
			auto *task = this->create_rpc_client_task("Login", std::move(done));

			task->serialize_input(req);
			task->start();
		}

		inline void SRPCHttpClient::Login(const ReqLogin *req, ResqLogin *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_Login(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<ResqLogin, srpc::RPCSyncContext>> SRPCHttpClient::async_Login(const ReqLogin *req)
		{
			using RESULT = std::pair<ResqLogin, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<ResqLogin>("Login", srpc::RPCAsyncFutureCallback<ResqLogin>);

			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::SRPCHttpClientTask *SRPCHttpClient::create_Login_task(LoginDone done)
		{
			return this->create_rpc_client_task("Login", std::move(done));
		}

		inline BRPCClient::BRPCClient(const char *host, unsigned short port) : srpc::BRPCClient("login.loginService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::BRPCClient::init(&params);
		}

		inline BRPCClient::BRPCClient(const struct srpc::RPCClientParams *params) : srpc::BRPCClient("login.loginService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::BRPCClient::init(temp);
		}

		inline void BRPCClient::Login(const ReqLogin *req, LoginDone done)
		{
			auto *task = this->create_rpc_client_task("Login", std::move(done));

			task->serialize_input(req);
			task->start();
		}

		inline void BRPCClient::Login(const ReqLogin *req, ResqLogin *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_Login(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<ResqLogin, srpc::RPCSyncContext>> BRPCClient::async_Login(const ReqLogin *req)
		{
			using RESULT = std::pair<ResqLogin, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<ResqLogin>("Login", srpc::RPCAsyncFutureCallback<ResqLogin>);

			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::BRPCClientTask *BRPCClient::create_Login_task(LoginDone done)
		{
			return this->create_rpc_client_task("Login", std::move(done));
		}

		inline TRPCClient::TRPCClient(const char *host, unsigned short port) : srpc::TRPCClient("login.loginService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::TRPCClient::init(&params);
		}

		inline TRPCClient::TRPCClient(const struct srpc::RPCClientParams *params) : srpc::TRPCClient("login.loginService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::TRPCClient::init(temp);
		}

		inline void TRPCClient::Login(const ReqLogin *req, LoginDone done)
		{
			auto *task = this->create_rpc_client_task("/login.loginService/Login", std::move(done));

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->start();
		}

		inline void TRPCClient::Login(const ReqLogin *req, ResqLogin *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_Login(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<ResqLogin, srpc::RPCSyncContext>> TRPCClient::async_Login(const ReqLogin *req)
		{
			using RESULT = std::pair<ResqLogin, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<ResqLogin>("/login.loginService/Login", srpc::RPCAsyncFutureCallback<ResqLogin>);

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::TRPCClientTask *TRPCClient::create_Login_task(LoginDone done)
		{
			auto *task = this->create_rpc_client_task("/login.loginService/Login", std::move(done));

			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);

			return task;
		}

		inline TRPCHttpClient::TRPCHttpClient(const char *host, unsigned short port) : srpc::TRPCHttpClient("login.loginService")
		{
			struct srpc::RPCClientParams params = srpc::RPC_CLIENT_PARAMS_DEFAULT;

			params.host = host;
			params.port = port;
			this->srpc::TRPCHttpClient::init(&params);
		}

		inline TRPCHttpClient::TRPCHttpClient(const struct srpc::RPCClientParams *params) : srpc::TRPCHttpClient("login.loginService")
		{
			const struct srpc::RPCClientParams *temp = params;
			struct srpc::RPCClientParams temp_params;

			this->srpc::TRPCHttpClient::init(temp);
		}

		inline void TRPCHttpClient::Login(const ReqLogin *req, LoginDone done)
		{
			auto *task = this->create_rpc_client_task("Login", std::move(done));

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->start();
		}

		inline void TRPCHttpClient::Login(const ReqLogin *req, ResqLogin *resp, srpc::RPCSyncContext *sync_ctx)
		{
			auto res = this->async_Login(req).get();

			if (resp && res.second.success)
				*resp = std::move(res.first);

			if (sync_ctx)
				*sync_ctx = std::move(res.second);
		}

		inline WFFuture<std::pair<ResqLogin, srpc::RPCSyncContext>> TRPCHttpClient::async_Login(const ReqLogin *req)
		{
			using RESULT = std::pair<ResqLogin, srpc::RPCSyncContext>;
			auto *pr = new WFPromise<RESULT>();
			auto fr = pr->get_future();
			auto *task = this->create_rpc_client_task<ResqLogin>("Login", srpc::RPCAsyncFutureCallback<ResqLogin>);

			if (this->params.callee_timeout >= 0)
				task->get_req()->set_callee_timeout(this->params.callee_timeout);
			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);
			task->serialize_input(req);
			task->user_data = pr;
			task->start();
			return fr;
		}

		inline srpc::TRPCHttpClientTask *TRPCHttpClient::create_Login_task(LoginDone done)
		{
			auto *task = this->create_rpc_client_task("Login", std::move(done));

			if (!this->params.caller.empty())
				task->get_req()->set_caller_name(this->params.caller);

			return task;
		}

	} // end namespace loginService

} // end namespace login
