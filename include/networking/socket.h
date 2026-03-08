#include <netdb.h>
#include <sys/socket.h>

#include <string>

namespace net {

class Socket {
 public:
  virtual ~Socket();

 protected:
  Socket(addrinfo hints);

  virtual void bind() = 0;
  virtual void connect() = 0;
  virtual void listenAndAccept() = 0;

  addrinfo m_hints;
  int m_socket_fd;
};

}  // namespace net