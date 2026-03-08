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