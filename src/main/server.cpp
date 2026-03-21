#include <iostream>

#include "networking/tcp_server.h"

int main() {
  try {
    net::TcpServer server("localhost", "8080");
    server.listenAndServe();
  } catch (const net::SocketException& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}