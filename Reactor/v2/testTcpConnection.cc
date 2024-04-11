#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include <iostream>
#include <unistd.h>
using std::cout;
using std::endl;

void onNewConnect(const TcpConnectionPtr &con) {
  cout << con->toString() << "has connected. Welcome to EchoCloud!" << endl;
}
void onMessage(const TcpConnectionPtr &con) {
  string msg = con->receive();
  cout << ">>receive from client message is " << msg << endl;

  string msg1("EchoCloud:");
  string cloud = u8"\U0001F329";
  msg1 += cloud;
  msg1 += msg;
  con->send(msg1);
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

int main(int argc, char **argv) {
  test();
  return 0;
}
