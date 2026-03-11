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
      m_family(AF_INET),
      m_type(SOCK_STREAM),
      m_protocol(0) {
  std::memset(&m_storage, 0, m_len);

  addrinfo hints;
  std::memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = m_family;
  hints.ai_socktype = m_type;
  hints.ai_protocol = m_protocol;

  addrinfo* address_info;
  auto result = getaddrinfo(ip.c_str(), port.c_str(), &hints, &address_info);

  auto context = std::string("[TcpSocket::TcpSocket] ");
  if (result != 0) {
    throw SocketException(context, gai_strerror(result));
  }

  std::memcpy(&m_storage, address_info->ai_addr, address_info->ai_addrlen);
  freeaddrinfo(address_info);

  m_socket_fd = socket(m_family, m_type, m_protocol);

  if (m_socket_fd == -1) {
    throw SocketException(context, strerror(errno));
  }
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
