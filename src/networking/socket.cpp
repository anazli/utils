#include "networking/socket.h"

#include <unistd.h>

#include <cerrno>
#include <cstring>

net::SocketException::SocketException(const std::string& context,
                                      int error_code)
    : std::runtime_error("Socket Error: "),
      m_context(context),
      m_errno(error_code) {
  std::string error_msg = gai_strerror(m_errno);
  m_context += error_msg;
}

const char* net::SocketException::what() const noexcept {
  return m_context.c_str();
}

int net::SocketException::getErrorCode() const { return m_errno; }

net::TcpSocket::TcpSocket(const std::string& ip, const std::string& port) {
  addrinfo hints;
  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;

  auto result = getaddrinfo(ip.c_str(), port.c_str(), &hints, &m_hints);

  auto context = std::string("[TcpSocket::TcpSocket] ");
  if (result != 0) {
    throw SocketException(context, result);
  }

  m_socket_fd =
      socket(m_hints->ai_family, m_hints->ai_socktype, m_hints->ai_protocol);
  if (m_socket_fd == -1) {
    throw SocketException(context, errno);
  }
}

net::TcpSocket::~TcpSocket() {
  if (m_socket_fd != -1) close(m_socket_fd);
}

int net::TcpSocket::getHandle() const { return m_socket_fd; }

int net::TcpSocket::getType() const { return m_hints->ai_socktype; }

int net::TcpSocket::getFamily() const { return m_hints->ai_family; }
