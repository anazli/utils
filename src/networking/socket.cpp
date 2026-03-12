#include "networking/socket.h"

#include <unistd.h>

#include <cstring>

net::SocketException::SocketException(const std::string& context,
                                      const std::string& error)
    : std::runtime_error("Socket Error: "), m_context(context) {
  m_context += error;
}

const char* net::SocketException::what() const noexcept {
  return m_context.c_str();
}

net::TcpSocket::TcpSocket(const std::string& ip, const std::string& port)
    : m_len(sizeof(m_storage)),
      m_family(AF_INET6),
      m_type(SOCK_STREAM),
      m_protocol(0) {
  std::memset(&m_storage, 0, m_len);

  addrinfo hints;
  std::memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = m_family;
  hints.ai_socktype = m_type;
  hints.ai_protocol = m_protocol;
  hints.ai_flags = AI_PASSIVE | AI_V4MAPPED | AI_ALL;  // accept any connections

  auto context = std::string("[TcpSocket::TcpSocket] ");

  addrinfo* address_info;

  if (auto result =
          getaddrinfo(ip.c_str(), port.c_str(), &hints, &address_info);
      result != 0) {
    throw SocketException(context, gai_strerror(result));
  }

  std::memcpy(&m_storage, address_info->ai_addr, address_info->ai_addrlen);
  freeaddrinfo(address_info);

  if (m_socket_fd = socket(m_family, m_type, m_protocol); m_socket_fd == -1) {
    throw SocketException(context, strerror(errno));
  }

  configureDualStack();
}

net::TcpSocket::TcpSocket(TcpSocket&& other)
    : m_storage(other.m_storage),
      m_len(other.m_len),
      m_socket_fd(other.m_socket_fd),
      m_family(other.m_family),
      m_type(other.m_type),
      m_protocol(other.m_protocol) {
  other.m_socket_fd = -1;
}

net::TcpSocket& net::TcpSocket::operator=(TcpSocket&& other) {
  if (this != &other) {
    if (m_socket_fd != -1) ::close(m_socket_fd);

    m_storage = other.m_storage;
    m_len = other.m_len;
    m_socket_fd = other.m_socket_fd;
    m_family = other.m_family;
    m_type = other.m_type;
    m_protocol = other.m_protocol;

    other.m_socket_fd = -1;
  }
  return *this;
}

net::TcpSocket::~TcpSocket() {
  if (m_socket_fd != -1) close(m_socket_fd);
}

int net::TcpSocket::getHandle() const { return m_socket_fd; }

int net::TcpSocket::getType() const { return m_type; }

int net::TcpSocket::getFamily() const { return m_family; }

net::TcpSocket::TcpSocket(int existing_fd, sockaddr_storage addr, socklen_t len)
    : m_socket_fd(existing_fd),
      m_storage(addr),
      m_len(len),
      m_family(addr.ss_family) {
  int type;
  socklen_t type_len = sizeof(type);
  getsockopt(m_socket_fd, SOL_SOCKET, SO_TYPE, &type, &type_len);
  m_type = type;

  int protocol;
  socklen_t prot_len = sizeof(protocol);
  getsockopt(m_socket_fd, SOL_SOCKET, SO_PROTOCOL, &protocol, &prot_len);
  m_protocol = protocol;
}

void net::TcpSocket::configureDualStack() {
  auto context = std::string("[TcpSocket::configureDualStack] ");
  int option{0};  // removes the IPV6_V6ONLY option for the socket
  if (setsockopt(m_socket_fd, IPPROTO_IPV6, IPV6_V6ONLY,
                 static_cast<void*>(&option), sizeof(option)) == -1) {
    throw SocketException(context, "Failed to configure dual stack!");
  }
}