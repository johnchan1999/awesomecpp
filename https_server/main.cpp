#include "server.h"

int main() {
  Server server("103.185.249.166", 8888);

  // Server server("127.0.0.1", 1234);
  server.ready();
  server.connectAndShow();
  return 0;
}
