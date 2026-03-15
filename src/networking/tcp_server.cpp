#include "networking/tcp_server.h"

#include <cstring>

net::TcpServer::TcpServer(const std::string& local_address,
                          const std::string& port)
    : Socket(EndpointAddress(local_address, port, SocketType::TYPE_TCP),
             SocketType::TYPE_TCP, Protocol::PROT_TCP) {}

net::TcpServer::TcpServer(const EndpointAddress& address)
    : Socket(address, SocketType::TYPE_TCP, Protocol::PROT_TCP) {}

void net::TcpServer::bind() {
  int opt = 1;  // prevent address already in use after restart
  setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (::bind(m_socket_fd, reinterpret_cast<sockaddr*>(m_address.getSockAddr()),
             *m_address.getLen()) == -1) {
    throw SocketException("[TcpServer::bind]", strerror(errno));
  }
}

void net::TcpServer::listen(int backlog) {
  if (::listen(m_socket_fd, backlog) == -1) {
    throw SocketException("[TcpServer::listen]", strerror(errno));
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
