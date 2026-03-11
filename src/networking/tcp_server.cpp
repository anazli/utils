#include "networking/tcp_server.h"

#include <cstring>

net::TcpServer::TcpServer(const std::string& local_address,
                          const std::string& port)
    : TcpSocket(local_address, port) {}

void net::TcpServer::bind() {
  if (::bind(m_socket_fd, m_address_info->ai_addr,
             m_address_info->ai_addrlen) == -1) {
    throw SocketException("[TcpSocket::bind]", strerror(errno));
  }
}