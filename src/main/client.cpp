#include <iostream>

#include "networking/tcp_server.h"

bool shouldExit(const std::string& message) {
  return message == "exit" || message == "quit";
}

int main() {
  net::TcpClient client;
  net::EndpointAddress remote_address("localhost", "8080");
  client.connect(remote_address);
  std::string message;
  std::cout << "Enter message to send:" << std::endl;
  while (!shouldExit(message)) {
    try {
      std::getline(std::cin, message);
      net::DataStream msg_to_send;
      msg_to_send.append(message);
      client.send(msg_to_send);

      net::DataStream received_msg(1024);
      client.recv(received_msg);

      std::cout /*<< "Succesfully received from server: "*/
          << received_msg.toString() << std::endl;

    } catch (const net::SocketException& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}