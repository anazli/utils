#include <print>

#include "networking/tcp_server.h"

int main() {
  try {
    net::TcpServer server("localhost", "8080");
    server.listenAndServe();
  } catch (const net::SocketException& e) {
    std::println(stderr, "{}", e.what());
  }

  return 0;
}