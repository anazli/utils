#include "networking/udp_server.h"

#include <cstring>

net::UdpServer::UdpServer(const std::string& local_address,
                          const std::string& port)
    : Socket(EndpointAddress::TYPE_UDP, EndpointAddress::PROT_UDP),
      m_address(local_address, port, EndpointAddress::TYPE_UDP) {}

void net::UdpServer::bind() {
  int opt = 1;  // prevent address already in use after restart
  setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (::bind(m_socket_fd, reinterpret_cast<sockaddr*>(m_address.getSockAddr()),
             *m_address.getLen()) == -1) {
    throw SocketException("[UdpServer::bind]", strerror(errno));
  }
}