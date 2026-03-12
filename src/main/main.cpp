#include <iostream>

#include "networking/tcp_server.h"

// Playgroud

int main() {
  try {
    net::TcpServer server("localhost", "8080");
    server.bind();
    std::cout << "Server is listening..." << std::endl;
    server.listen(1);
    std::cout << "Done!" << std::endl;
    std::cout << "Accepting new connection" << std::endl;
    auto client = server.accept();
    std::cout << "On file descriptor:" << client.getHandle() << std::endl;
  } catch (const net::SocketException& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}