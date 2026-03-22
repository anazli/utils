#include <iostream>
#include <print>
#include <thread>

#include "networking/tcp_server.h"

bool shouldExit(const std::string& message) {
  return message == "exit" || message == "quit";
}

void show_prompt() {
  std::print("\033[1;32m>\033[0m ");
  std::fflush(stdout);
}

int main() {
  net::TcpClient client;
  net::EndpointAddress remote_address("localhost", "8080");
  client.connect(remote_address);

  std::thread listener([&client] {
    while (true) {
      try {
        net::DataStream received_msg(1024);
        if (auto bytes_received = client.recv(received_msg);
            bytes_received == 0) {
          return EXIT_FAILURE;
        }
        std::println("{}", /*<< "Succesfully received from server: "*/
                     received_msg.toString());
        show_prompt();
      } catch (const net::SocketException& e) {
        std::println(stderr, "{}", e.what());
        return EXIT_FAILURE;
      }
    };
  });
  listener.detach();

  std::string message;
  std::println("Enter message to send:");
  show_prompt();

  while (!shouldExit(message)) {
    try {
      std::getline(std::cin, message);

      net::DataStream msg_to_send;
      msg_to_send.append(message);

      client.send(msg_to_send);
      show_prompt();
    } catch (const net::SocketException& e) {
      std::println(stderr, "{}", e.what());
    }
  }

  return 0;
}