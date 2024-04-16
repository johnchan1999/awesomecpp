#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "ThreadPool.h"
#include <cctype>
#include <iostream>
#include <unistd.h>
using std::cout;
using std::endl;

ThreadPool *gPool = nullptr;

class MyTask {
public:
  MyTask(const string &msg, const TcpConnectionPtr &con)
      : _msg(msg), _con(con) {}
  void process() {
    toUpper();
    _con->sendInLoop(_msg);
  }

  void toUpper() {
    for (char &c : _msg) {
      std::toupper(c);
    }
  }

private:
  string _msg;
  TcpConnectionPtr _con;
};

void onNewConnect(const TcpConnectionPtr &con) {
  cout << con->toString() << "has connected. Welcome to EchoCloud!" << endl;
}
void onMessage(const TcpConnectionPtr &con) {
  string msg = con->receive();
  cout << ">>receive from client message is " << msg << endl;
  string msg1 = u8"\U0001F300";
  string msg2("EchoCloud:");
  msg1 += msg2;
  string cloud = u8"\U0001F63B";
  msg1 += cloud;
  msg1 += msg;
  // con->send(msg1);
  MyTask task(msg1, con);
  gPool->addTask(std::bind(&MyTask::process, task));
}
void onClose(const TcpConnectionPtr &con) {

  cout << con->toString()
       << "has disconnected.Have a good day!Byebye @EchoCloud!" << endl;
}

void test() {
  Acceptor acceptor("127.0.0.1", 8888);
  acceptor.ready(); // 此时处于监听状态

  // 三次握手就已经建立，可以创建一条TCP连接
  EventLoop Loop(acceptor);
  Loop.setNewConnectCallBack(std::move(onNewConnect));
  Loop.setMessageCallBack(std::move(onMessage));
  Loop.setCloseCallBack(std::move(onClose));
  Loop.Loop();
}

void test1() {
  TcpServer tcpserver("127.0.0.1", 8888);
  tcpserver.setAllCallBack(std::move(onNewConnect), std::move(onMessage),
                           std::move(onClose));
  tcpserver.start();
}

void test2() {
  ThreadPool pool(4, 10);
  pool.start();

  gPool = &pool;

  TcpServer tcpserver("127.0.0.1", 8888);
  tcpserver.setAllCallBack(std::move(onNewConnect), std::move(onMessage),
                           std::move(onClose));
  tcpserver.start();
}

int main(int argc, char **argv) {
  test2();
  return 0;
}
