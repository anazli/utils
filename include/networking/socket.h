#include <netdb.h>
#include <sys/socket.h>

#include <string>

namespace net {

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