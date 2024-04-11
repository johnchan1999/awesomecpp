#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__
#include "InetAddress.h"
#include "Socket.h"
#include <string.h>

using std::string;

class Acceptor {
public:
  Acceptor(const string ip, unsigned short port);
  ~Acceptor();
  void ready();
  int accept();

private:
  void setReuseAddr();
  void setReusePort();
  void bind();
  void listen();

public:
  int fd();

private:
  Socket _socket;
  InetAddress _InetAdress;
};

#endif
