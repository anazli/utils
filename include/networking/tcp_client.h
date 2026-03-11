#pragma once

#include "networking/socket.h"

namespace net {

class TcpClient : public TcpSocket {
 public:
  /*
   * Creates a TCP Client for the given host address and port number
   * to connect to
   * @param host IP address or hostname to connect
   * @param port Port number as string
   * @throws SocketException if address resolution or socket creation fails
   */
  TcpClient(const std::string& host, const std::string& port);

  TcpClient(const TcpClient&) = delete;
  TcpClient& operator=(TcpClient&) = delete;

  TcpClient(TcpClient&& other) = default;
  TcpClient& operator=(TcpClient&& other) = default;

  ~TcpClient() override = default;

  /*
   * Connects the socket to the provided address during construction
   */
  void connect();

 private:
};

}  // namespace net