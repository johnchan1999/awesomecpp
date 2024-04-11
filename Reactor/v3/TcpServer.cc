#include "TcpServer.h"

TcpServer::TcpServer(string ip, unsigned short port)
    : _acceptor(ip, port), _eventloop(_acceptor) {}

TcpServer::~TcpServer() {}

void TcpServer::start() {
  _acceptor.ready();
  _eventloop.Loop();
}

void TcpServer::stop() { _eventloop.unLoop(); }

void TcpServer::setAllCallBack(TcpConnectionCallBack &&cb1,
                               TcpConnectionCallBack &&cb2,
                               TcpConnectionCallBack &&cb3) {
  _eventloop.setNewConnectCallBack(std::move(cb1));
  _eventloop.setMessageCallBack(std::move(cb2));
  _eventloop.setCloseCallBack(std::move(cb3));
}
