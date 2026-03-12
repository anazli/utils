#include <iostream>

#include "networking/tcp_server.h"

// Playgroud

int main() {
  try {
    net::TcpClient client("localhost", "8080");
    net::DataPacket msg_to_send;
    msg_to_send.append("Howdy, TCP Server");

    std::cout << "Client connects to the server.." << std::endl;
    client.connect();

    std::cout << "Client sends message to the server.." << std::endl;
    client.send(msg_to_send);

    std::cout << "Client receives message from the server.." << std::endl;
    net::DataPacket received_msg(1024);
    client.recv(received_msg);

    std::cout << "Succesfully received from server: " << received_msg.toString()
              << std::endl;

  } catch (const net::SocketException& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}