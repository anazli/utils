#pragma once

#include "socket.h"

namespace net {

class UdpServer : public Socket {
 public:
  /*
   * Creates a UDP Server for the given local address and port number
   * to bind to
   * @param local_address IP address or hostname to bind
   * @param port Port number as string
   * @throws SocketException if address resolution or socket creation fails
   */
  UdpServer(const std::string& local_address, const std::string& port);

  UdpServer(const UdpServer&) = delete;
  UdpServer& operator=(const UdpServer&) = delete;

  UdpServer(UdpServer&& other) noexcept = default;
  UdpServer& operator=(UdpServer&& other) noexcept = default;

  ~UdpServer() override = default;

  /*
   * Binds the server to a local address
   */
  void bind();

 private:
};

}  // namespace net