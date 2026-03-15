#pragma once

#include "socket.h"

namespace net {
class UdpClient : Socket {
 public:
  UdpClient() = default;
  /*
   * Creates a UDP Client for the given host address and port number
   * to connect to
   * @param host IP address or hostname to connect
   * @param port Port number as string
   * @throws SocketException if address resolution or socket creation fails
   */
  UdpClient(const std::string& host, const std::string& port);

  UdpClient(const UdpClient&) = delete;
  UdpClient& operator=(UdpClient&) = delete;

  UdpClient(UdpClient&& other) noexcept = default;
  UdpClient& operator=(UdpClient&& other) noexcept = default;

  ~UdpClient() override = default;

  ssize_t sendTo(const DataStream& stream, EndpointAddress& address);
  ssize_t recvFrom(DataStream& stream, EndpointAddress& address,
                   size_t stream_size);

 private:
};
}  // namespace net