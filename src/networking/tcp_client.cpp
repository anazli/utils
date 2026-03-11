#include "networking/tcp_client.h"

#include <cstring>

net::TcpClient::TcpClient(const std::string& host, const std::string& port)
    : TcpSocket(host, port) {}

void net::TcpClient::connect() {
  if (::connect(m_socket_fd, reinterpret_cast<sockaddr*>(&m_storage), m_len) ==
      -1) {
    throw SocketException("[TcpSocket::bind]", strerror(errno));
  }
}