#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <pthread.h>

class Socket {
public:
  explicit Socket(int fd);
  Socket();
  ~Socket();
  int fd();

private:
  int _fd;
};
#endif
