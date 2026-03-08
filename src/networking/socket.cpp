#include "networking/socket.h"

#include <unistd.h>

net::Socket::~Socket() {
  if (m_socket_fd != -1) close(m_socket_fd);
}

int net::Socket::getHandle() const { return m_socket_fd; }

int net::Socket::getType() const { return m_hints.ai_socktype; }

int net::Socket::getFamily() const { return m_hints.ai_family; }

net::Socket::Socket(int family, int type, int protocol) {
  m_socket_fd = socket(family, type, protocol);
  if (m_socket_fd == -1) {
    // TODO throw;
  }
}

net::TcpClient::TcpClient(const std::string& ip, int port)
    : Socket(AF_INET, SOCK_STREAM, 0) {}