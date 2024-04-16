#ifndef __SERVER_H__
#define __SERVER_H__
#include <string>
using std::string;

class Server {
public:
  Server(const string &ip, unsigned short port);

  ~Server();
  void ready();

  void connectAndShow();

private:
  string response();
  void MyBind();

private:
  const string _ip;
  unsigned short _port;
  int _socketfd;
  bool _isLoop;
};

#endif
