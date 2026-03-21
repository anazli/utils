#pragma once

#include "networking/socket.h"

namespace net {

class TcpClient : public Socket {
 public:
  TcpClient();
  TcpClient(int existing_fd, const EndpointAddress& remote_address);

  TcpClient(const TcpClient&) = delete;
  TcpClient& operator=(TcpClient&) = delete;

  TcpClient(TcpClient&& other) noexcept = default;
  TcpClient& operator=(TcpClient&& other) noexcept = default;

  ~TcpClient() override = default;

  EndpointAddress& getLocalAddress() override;

  /*
   * Connects the socket to the remote address
   */
  void connect(const EndpointAddress& remote_address);

  /*
   * Sends message to the connected endpoint
   * @param message The message to send
   * @param flags Flags for the system call
   * @return The number of bytes sent if result > 0
   * if interrupted return equals to 0
   * throws SocketException otherwise
   */
  ssize_t send(const DataStream& message, int flags = 0);

  /*
   * Receives message from the connected endpoint
   * @param message The message to receive (space must already be allocated)
   * @param flags Flags for the system call
   * @return The number of bytes received if result > 0
   * if connection closed return equals to 0
   * throws SocketException if message buffer is empty or if result < 0
   */
  ssize_t recv(DataStream& message, int flags = 0);
};

}  // namespace net