#pragma once

#include <netdb.h>
#include <sys/socket.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace net {

class SocketException : public std::runtime_error {
 public:
  SocketException(const std::string& context, const std::string& error);

  const char* what() const noexcept override;

 private:
  std::string m_context;
};

class DataPacket {
 public:
  DataPacket() = default;
  explicit DataPacket(size_t size);
  DataPacket(const uint8_t* data, size_t len);
  DataPacket(std::initializer_list<uint8_t> list);

  DataPacket& append(const uint8_t* data, size_t len);
  DataPacket& append(const std::string& data);
  const uint8_t* data() const;
  uint8_t* data();
  size_t size() const;
  void clear();
  void resize(size_t size);
  std::string toString() const;

 private:
  std::vector<uint8_t> m_buffer;
};

// A TCP Socket for client/server communication
class TcpSocket {
 public:
  /*
   * Creates a socket for the given ip and port number
   * @param ip IP address or hostname to resolve
   * @param port Port number as string
   * @throws SocketException if address resolution or socket creation fails
   */
  TcpSocket(const std::string& ip, const std::string& port);

  TcpSocket(const TcpSocket&) = delete;
  TcpSocket& operator=(const TcpSocket&) = delete;

  TcpSocket(TcpSocket&& other) noexcept;
  TcpSocket& operator=(TcpSocket&& other) noexcept;

  /*
   * Closes the socket and frees all resources
   */
  virtual ~TcpSocket();

  /*
   * Sends message to the connected endpoint
   * @param message The message to send
   * @param flags Flags for the system call
   * @return The number of bytes sent if result > 0
   * if interrupted return equals to 0
   * throws SocketException otherwise
   */
  ssize_t send(const DataPacket& message, int flags = 0);

  /*
   * Receives message from the connected endpoint
   * @param message The message to receive (space must already be allocated)
   * @param flags Flags for the system call
   * @return The number of bytes received if result > 0
   * if connection closed return equals to 0
   * throws SocketException if message buffer is empty or if result < 0
   */
  ssize_t recv(DataPacket& message, int flags = 0);

  /*
   * Returns the socket handle
   */
  int getHandle() const;

  /*
   * Returns the socket type (TCP)
   */

  int getType() const;

  /*
   * Returns the socket family/domain
   */
  int getFamily() const;

  /*
   * Returns the socket Protocol (/etc/protocols, i.e 6=TCP)
   */
  int getProtocol() const;

 protected:
  TcpSocket() = default;
  TcpSocket(int existing_fd, sockaddr_storage addr, socklen_t len);

  void configureDualStack();

  sockaddr_storage m_storage;
  socklen_t m_len;
  int m_socket_fd;
  int m_family;
  int m_type;
  int m_protocol;
};

}  // namespace net