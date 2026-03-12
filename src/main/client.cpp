#include <iostream>

#include "networking/tcp_server.h"

// Playgroud

int main(int argc, char** argv) {
  // Get message from cli to send
  std::string arg_msg;
  if (argc == 2) {
    arg_msg = argv[1];
  }

  try {
    net::TcpClient client("localhost", "8080");
    net::DataStream msg_to_send;
    msg_to_send.append(arg_msg);

    std::cout << "Client connects to the server.." << std::endl;
    client.connect();

    std::cout << "Client sends message to the server.." << std::endl;
    client.send(msg_to_send);

    std::cout << "Client receives message from the server.." << std::endl;
    net::DataStream received_msg(1024);
    client.recv(received_msg);

    std::cout << "Succesfully received from server: " << received_msg.toString()
              << std::endl;

  } catch (const net::SocketException& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}