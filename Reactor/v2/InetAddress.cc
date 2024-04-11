#include "InetAddress.h"
#include <strings.h>
InetAddress::InetAddress(const string &ip, unsigned short port) {
  bzero(&_addr, sizeof(struct sockaddr_in));
  _addr.sin_family = AF_INET;
  _addr.sin_addr.s_addr = inet_addr(ip.c_str());
  _addr.sin_port = htons(port);
}

InetAddress::~InetAddress() {}
string InetAddress::ip() { return string(inet_ntoa(_addr.sin_addr)); }
unsigned short InetAddress::port() { return ntohs(_addr.sin_port); }
sockaddr_in *InetAddress::getInetAddressPtr() { return &_addr; }

InetAddress::InetAddress(const struct sockaddr_in &addr) : _addr(addr) {}
