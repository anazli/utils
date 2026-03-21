#pragma once

#include "socket.h"

namespace net {
class UdpClient : public Socket {
 public:
  UdpClient();

  UdpClient(const UdpClient&) = delete;
  UdpClient& operator=(UdpClient&) = delete;

  UdpClient(UdpClient&& other) noexcept = default;
  UdpClient& operator=(UdpClient&& other) noexcept = default;

  ~UdpClient() override = default;

  EndpointAddress& getLocalAddress() override;

  ssize_t sendTo(const DataStream& stream, EndpointAddress& address);
  ssize_t recvFrom(DataStream& stream, EndpointAddress& address,
                   size_t stream_size);
};
}  // namespace net