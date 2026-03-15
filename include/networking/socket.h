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

enum SocketType { TYPE_TCP = SOCK_STREAM, TYPE_UDP = SOCK_DGRAM };
enum Protocol { PROT_TCP = 6, PROT_UDP = 17 };

class EndpointAddress {
 public:
  EndpointAddress();
  EndpointAddress(const std::string& ip, const std::string& port,
                  SocketType type);

  const sockaddr* getSockAddr() const;
  sockaddr* getSockAddr();

  const socklen_t* getLen() const;
  socklen_t* getLen();

  SocketType getSockType() const;

 private:
  sockaddr_storage m_storage;
  socklen_t m_storage_len;
  SocketType m_sock_type;
};

class DataStream {
 public:
  DataStream() = default;
  explicit DataStream(size_t size);
  DataStream(const uint8_t* data, size_t len);
  DataStream(std::initializer_list<uint8_t> list);

  DataStream& append(const uint8_t* data, size_t len);
  DataStream& append(const std::string& data);
  const uint8_t* data() const;
  uint8_t* data();
  size_t size() const;
  void clear();
  void resize(size_t size);
  std::string toString() const;

 private:
  std::vector<uint8_t> m_buffer;
};

// A Socket for client/server communication
class Socket {
 public:
  /*
   * Creates a socket for the given ip and port number
   * @param ip IP address or hostname to resolve
   * @param port Port number as string
   * @throws SocketException if address resolution or socket creation fails
   */
  Socket(const EndpointAddress& address, SocketType type, Protocol protocol);

  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;

  Socket(Socket&& other) noexcept;
  Socket& operator=(Socket&& other) noexcept;

  /*
   * Closes the socket and frees all resources
   */
  virtual ~Socket();

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

  const EndpointAddress& getAddress() const;
  EndpointAddress& getAddress();

 protected:
  Socket() = default;
  Socket(int existing_fd, sockaddr_storage addr, socklen_t len);

  void configureDualStack();

  EndpointAddress m_address;
  int m_socket_fd;
  int m_family;
  int m_type;
  int m_protocol;
};

}  // namespace net