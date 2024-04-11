#include "SocketIO.h"
#include "stdio.h"
SocketIO::SocketIO(int fd) : _fd(fd) {}
SocketIO::~SocketIO() { close(_fd); }
int SocketIO::readn(char *buf, int len) {
  int left = len;
  char *ptr = buf;
  int ret = 0;
  while (left) {
    ret = read(_fd, ptr, left);
    if (ret == -1 && errno == EINTR) {
      continue;
    }
    if (ret == -1) {

      perror("read erro -1 ");
      return -1;
    }
    if (ret == 0) {
      break;
    } else {
      ptr += ret;
      left -= ret;
    }
  }
  return len;
}

int SocketIO::writen(const char *buf, int len) {
  int left = len;
  const char *pstr = buf;
  int ret = 0;
  while (left) {
    ret = write(_fd, buf, left);
    if (ret == -1 && errno == EINTR) {
      continue;
    }

    if (ret == -1) {
      perror("write errno -1 ");
      return -1;
    }
    if (ret == 0) {
      break;

    } else {
      pstr += ret;
      left -= ret;
    }
  }
  return len;
}

int SocketIO::readLine(char *buf, int len) {
  int left = len - 1;
  int ret = 0;
  char *pstr = buf;
  int total = 0;

  while (left) {
    ret = ::recv(_fd, pstr, left, MSG_PEEK);
    if (ret == -1 && errno == EINTR) {
      continue;
    }
    if (ret == -1) {
      return -1;
    }
    if (ret == 0) {
      break;
    } else {
      for (int idx = 0; idx < ret; idx++) {
        if (pstr[idx] == '\n') {
          int sz = idx + 1;
          readn(buf, sz);
          pstr += sz;
          *pstr = '\0';
          return total + sz;
        }
      }
      readn(pstr, ret);
      total += ret;
      left -= ret;
      pstr += ret;
    }
  }
  *pstr = '\0';
  return total;
}
