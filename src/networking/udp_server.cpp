#include "networking/udp_server.h"

#include <cstring>

net::UdpServer::UdpServer(const std::string& local_address,
                          const std::string& port)
    : Socket(EndpointAddress(local_address, port, SocketType::TYPE_UDP),
             SocketType::TYPE_UDP, Protocol::PROT_UDP) {}

net::UdpServer::UdpServer(const EndpointAddress& address)
    : Socket(address, SocketType::TYPE_UDP, Protocol::PROT_UDP) {}

void net::UdpServer::bind() {
  int opt = 1;  // prevent address already in use after restart
  setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (::bind(m_socket_fd,
             reinterpret_cast<sockaddr*>(m_local_address.getSockAddr()),
             *m_local_address.getLen()) == -1) {
    throw SocketException("[UdpServer::bind]", strerror(errno));
  }
}

ssize_t net::UdpServer::sendTo(const DataStream& stream,
                               EndpointAddress& address) {
  int bytes_sent = ::sendto(m_socket_fd, stream.data(), stream.size(), 0,
                            address.getSockAddr(), *address.getLen());
  if (bytes_sent == -1) {
    throw SocketException("[UdpServer::sendTo]", strerror(errno));
  }
  return bytes_sent;
}

ssize_t net::UdpServer::recvFrom(DataStream& stream, EndpointAddress& address,
                                 size_t stream_size) {
  stream.resize(stream_size);
  int bytes_received = ::recvfrom(m_socket_fd, stream.data(), stream.size(), 0,
                                  address.getSockAddr(), address.getLen());
  if (bytes_received < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return 0;
    }
    throw SocketException("[UdpServer::recvFrom]", strerror(errno));
  }
  stream.resize(static_cast<ssize_t>(bytes_received));
  return bytes_received;
}