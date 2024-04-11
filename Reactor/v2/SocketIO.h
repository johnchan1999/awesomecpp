#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class SocketIO {
public:
  SocketIO(int fd);
  ~SocketIO();
  int readn(char *buf, int len);
  int writen(const char *buf, int len);
  int readLine(char *buf, int len);

private:
  int _fd;
};
#endif
