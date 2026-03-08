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

net::TcpSocket::TcpSocket(const std::string& ip, const std::string& port) {
  addrinfo hints;
  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;

  auto result = getaddrinfo(ip.c_str(), port.c_str(), &hints, &m_address_info);

  auto context = std::string("[TcpSocket::TcpSocket] ");
  if (result != 0) {
    throw SocketException(context, gai_strerror(result));
  }

  m_socket_fd = socket(m_address_info->ai_family, m_address_info->ai_socktype,
                       m_address_info->ai_protocol);
  if (m_socket_fd == -1) {
    freeaddrinfo(m_address_info);
    throw SocketException(context, strerror(errno));
  }
}

net::TcpSocket::~TcpSocket() {
  if (m_socket_fd != -1) close(m_socket_fd);
  if (m_address_info != nullptr) freeaddrinfo(m_address_info);
}

int net::TcpSocket::getHandle() const { return m_socket_fd; }

int net::TcpSocket::getType() const { return m_address_info->ai_socktype; }

int net::TcpSocket::getFamily() const { return m_address_info->ai_family; }

void net::TcpSocket::bind() {
  if (::bind(m_socket_fd, m_address_info->ai_addr,
             m_address_info->ai_addrlen) == -1) {
    throw SocketException("[TcpSocket::bind]", strerror(errno));
  }
}
