#include <netdb.h>
#include <sys/socket.h>

#include <stdexcept>
#include <string>

namespace net {

class SocketException : public std::runtime_error {
 public:
  SocketException(const std::string& context, const std::string& error);

  const char* what() const noexcept override;

 private:
  std::string m_context;
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

  /*
   * Closes the socket and frees all resources
   */
  ~TcpSocket();

  /*
   * Returns the socket handle
   */
  int getHandle() const;

  /*
   * Returns the socket type (TCP)
   */

  int getType() const;
  /*
   * Returns the socket family/domain (IPv4 for the moment)
   */
  int getFamily() const;

  /*
   * Binds the socket to a local address (for server usage)
   */
  void bind();

  /*
   * Accepts and listens to requests (for server usage)
   */
  void listenAndAccept() {};

 protected:
  addrinfo* m_address_info;
  int m_socket_fd;
};

}  // namespace net