#pragma once

#include "socket.h"

namespace net {

class TcpServer : public TcpSocket {
 public:
  /*
   * Creates a TCP Server for the given local address and port number
   * to bind to
   * @param local_address IP address or hostname to bind
   * @param port Port number as string
   * @throws SocketException if address resolution or socket creation fails
   */
  TcpServer(const std::string& local_address, const std::string& port);

  TcpServer(const TcpServer&) = delete;
  TcpServer& operator=(const TcpServer&) = delete;

  /*
   * Binds the server to a local address
   */
  void bind();

 private:
};

}  // namespace net