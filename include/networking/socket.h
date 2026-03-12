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

  void insert(const uint8_t* data, size_t len);
  const uint8_t* data() const;
  size_t size() const;
  void clear();

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