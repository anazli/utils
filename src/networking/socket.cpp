#include "networking/socket.h"

#include <unistd.h>

#include <cstring>

net::SocketException::SocketException(const std::string& context, int errno)
    : std::runtime_error("Socket Error: "),
      m_context(context),
      m_errno(errno) {}

const char* net::SocketException::what() const noexcept {
  std::string error_msg = strerror(errno);
  auto ret = m_context + error_msg;
  return ret.c_str();
}

int net::SocketException::getErrorCode() const { return m_errno; }

net::TcpSocket::TcpSocket(const std::string& ip, const std::string& port) {
  addrinfo hints;
  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;

  auto result = getaddrinfo(ip.c_str(), port.c_str(), &hints, &m_hints);

  if (result == 0) {
    // TODO throw
  }

  m_socket_fd =
      socket(m_hints->ai_family, m_hints->ai_socktype, m_hints->ai_protocol);
  if (m_socket_fd == -1) {
    // TODO throw;
  }
}

net::TcpSocket::~TcpSocket() {
  if (m_socket_fd != -1) close(m_socket_fd);
}

int net::TcpSocket::getHandle() const { return m_socket_fd; }

int net::TcpSocket::getType() const { return m_hints->ai_socktype; }

int net::TcpSocket::getFamily() const { return m_hints->ai_family; }
