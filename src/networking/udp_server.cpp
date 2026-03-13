#include "networking/udp_server.h"

#include <cstring>

net::UdpServer::UdpServer(const std::string& local_address,
                          const std::string& port)
    : Socket(local_address, port, SocketType::TYPE_UDP) {}

void net::UdpServer::bind() {
  int opt = 1;  // prevent address already in use after restart
  setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (::bind(m_socket_fd, reinterpret_cast<sockaddr*>(&m_storage), m_len) ==
      -1) {
    throw SocketException("[UdpServer::bind]", strerror(errno));
  }
}