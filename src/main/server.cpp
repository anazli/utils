#include <iostream>

#include "networking/tcp_server.h"

// Playgroud

int main() {
  try {
    net::TcpServer server("localhost", "8080");
    server.bind();
    std::cout << "Server is listening..." << std::endl;
    server.listen();
    std::cout << "Done!" << std::endl;

    net::DataStream received_msg(1024);
    while (received_msg.toString() != "Exit") {
      std::cout << "Accepting new connection" << std::endl;
      auto client = server.accept();
      std::cout << "On file descriptor:" << client.getHandle() << std::endl;

      client.recv(received_msg);

      std::cout << "Server received the following message: "
                << received_msg.toString() << std::endl;

      net::DataStream msg_to_send;
      msg_to_send.append("Hello Client, you sent: ")
          .append(received_msg.data(), received_msg.size());

      client.send(msg_to_send);
    }

  } catch (const net::SocketException& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}