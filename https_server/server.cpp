#include "server.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <memory.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using std::cout;
using std::endl;

#define ERROR -1
Server::Server(const string &ip, unsigned short port)
    : _ip(ip), _port(port), _socketfd(-1), _isLoop(false) {}
Server::~Server() {
  if (_socketfd > 0) {
    close(_socketfd);
  }
  _isLoop = false;
}

void Server::MyBind() {
  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr(_ip.c_str());
  serverAddr.sin_port = htons(_port);

  int ret =
      ::bind(_socketfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

  if (ret < 0) {
    perror("::bind");
    exit(ERROR);
  }
}
void Server::ready() {
  _socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (_socketfd < 0) {
    perror("socket");
    exit(ERROR);
  }

  int reuse = 1;
  int ret =
      setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  if (ret < 0) {
    perror("setsockopt");
    exit(ERROR);
  }

  MyBind();

  listen(_socketfd, 10);
}

void Server::connectAndShow() {
  _isLoop = true;
  while (_isLoop) {
    int netfd = accept(_socketfd, nullptr, nullptr);

    if (netfd < 0) {
      perror("accpet");
      exit(ERROR);
    }

    char buf[4096] = {0};

    ssize_t ret = recv(netfd, &buf, sizeof(buf), 0);
    if (ret < 0) {
      perror("recv");
      return;
    }

    cout << "ret=" << ret << ","
         << "buf=" << buf << endl;
    string resp = response();

    send(netfd, resp.c_str(), resp.size(), 0);
    close(netfd);
  }
}

string Server::response() {
  string start_line = "HTTP/1.1 200 OK\r\n";
  string headers = "Server EchoCloud\r\n";
  int fd = open("1.jpg", O_RDWR);
  if (fd < 0) {
    perror("open()");
    exit(ERROR);
  }
  off_t filesize = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  char *buf = new char[filesize];
  int ret = read(fd, &buf, filesize);
  if (ret < 0) {
    perror("read");
    close(fd);
    delete[] buf;
    exit(ERROR);
  }
  string body = buf;
  headers += "Content-Type: image/jpeg";
  headers += "Content-Length:" + std::to_string(body.size()) + "\r\n";
  string empty_line = "\r\n";
  return start_line + headers + empty_line + body;
}
