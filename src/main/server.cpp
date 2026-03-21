#include <iostream>

#include "networking/tcp_server.h"

bool shouldExit(const std::string& message) {
  return message == "exit" || message == "quit";
}

int main() {
  try {
    net::TcpServer server("localhost", "8080");
    server.bind();
    std::cout << "Server is listening..." << std::endl;
    server.listen();

    net::DataStream received_msg(1024);
    while (!shouldExit(received_msg.toString())) {
      std::cout << "Accepting new connection" << std::endl;
      auto client = server.accept();
      std::cout << "Client: " << client.getRemoteAddress().toString()
                << " connected.." << std::endl;

      client.recv(received_msg);

      std::cout << "Server received the following message: "
                << received_msg.toString() << std::endl;

      net::DataStream msg_to_send;
      msg_to_send.append("Server: ")
          .append(received_msg.data(), received_msg.size());

      client.send(msg_to_send);
    }

  } catch (const net::SocketException& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}