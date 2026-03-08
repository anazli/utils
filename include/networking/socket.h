#include <netdb.h>
#include <sys/socket.h>

#include <string>

namespace net {

class Socket {
 public:
  virtual ~Socket();

  int getHandle() const;
  int getType() const;
  int getFamily() const;

  virtual void bind() = 0;
  virtual void connect() {};
  virtual void listenAndAccept() {};

 protected:
  Socket(int family, int type, int protocol);

  addrinfo m_hints;
  int m_socket_fd;
};

class TcpClient : public Socket {
 public:
  TcpClient(const std::string& ip, int port);
  TcpClient(const TcpClient&) = delete;
  TcpClient& operator=(const TcpClient&) = delete;
  ~TcpClient() override = default;

  void bind() override {}

 private:
};

}  // namespace net