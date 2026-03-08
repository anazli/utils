#include <netdb.h>
#include <sys/socket.h>

#include <stdexcept>
#include <string>

namespace net {

class SocketException : public std::runtime_error {
 public:
  SocketException(const std::string& context, int errno);

  const char* what() const noexcept override;
  int getErrorCode() const;

 private:
  std::string m_context;
  int m_errno;
};

class TcpSocket {
 public:
  TcpSocket(const std::string& ip, const std::string& port);
  ~TcpSocket();

  int getHandle() const;
  int getType() const;
  int getFamily() const;

  void bind() {};
  void connect() {};
  void listenAndAccept() {};

 private:
  addrinfo* m_hints;
  int m_socket_fd;
};

}  // namespace net