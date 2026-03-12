#include "networking/tcp_server.h"

#include <cstring>

net::TcpServer::TcpServer(const std::string& local_address,
                          const std::string& port)
    : TcpSocket(local_address, port) {}

void net::TcpServer::bind() {
  int opt = 1;  // prevent address already in use after restart
  setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (::bind(m_socket_fd, reinterpret_cast<sockaddr*>(&m_storage), m_len) ==
      -1) {
    throw SocketException("[TcpSocket::bind]", strerror(errno));
  }
}

void net::TcpServer::listen(int backlog) {
  if (::listen(m_socket_fd, backlog) == -1) {
    throw SocketException("[TcpSocket::listen]", strerror(errno));
  }
}

net::TcpClient net::TcpServer::accept() {
  sockaddr_storage client_addr;
  socklen_t client_len = sizeof(client_addr);

  // blocking call
  int new_client = ::accept(
      m_socket_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);

  return TcpClient(new_client, client_addr, client_len);
}
