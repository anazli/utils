#include "networking/tcp_client.h"

#include <cstring>

net::TcpClient::TcpClient(const std::string& host, const std::string& port)
    : TcpSocket(host, port) {}

void net::TcpClient::connect() {
  if (::connect(m_socket_fd, m_address_info->ai_addr,
                m_address_info->ai_addrlen) == -1) {
    throw SocketException("[TcpSocket::bind]", strerror(errno));
  }
}