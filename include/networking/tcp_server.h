#pragma once

#include "socket.h"
#include "tcp_client.h"

namespace net {

class TcpServer : public Socket {
 public:
  /*
   * Creates a TCP Server for the given local address and port number
   * to bind to
   * @param local_address IP address or hostname to bind
   * @param port Port number as string
   * @throws SocketException if address resolution or socket creation fails
   */
  TcpServer(const std::string& local_address, const std::string& port);
  TcpServer(const EndpointAddress& address);

  TcpServer(const TcpServer&) = delete;
  TcpServer& operator=(const TcpServer&) = delete;

  TcpServer(TcpServer&& other) noexcept = default;
  TcpServer& operator=(TcpServer&& other) noexcept = default;

  ~TcpServer() override = default;

  /*
   * Binds the server to a local address
   */
  void bind();

  /*
   * Not able to call send or recv after this call
   */
  void listen(int backlog = SOMAXCONN);

  TcpClient accept();
};

}  // namespace net