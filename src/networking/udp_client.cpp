#include "networking/udp_client.h"

#include <cstring>

net::UdpClient::UdpClient()
    : Socket(SocketType::TYPE_UDP, Protocol::PROT_UDP) {}

ssize_t net::UdpClient::sendTo(const DataStream& stream,
                               EndpointAddress& address) {
  int bytes_sent = ::sendto(m_socket_fd, stream.data(), stream.size(), 0,
                            address.getSockAddr(), *address.getLen());
  if (bytes_sent == -1) {
    throw SocketException("[UdpClient::sendTo]", strerror(errno));
  }
  return bytes_sent;
}

ssize_t net::UdpClient::recvFrom(DataStream& stream, EndpointAddress& address,
                                 size_t stream_size) {
  stream.resize(stream_size);
  int bytes_received = ::recvfrom(m_socket_fd, stream.data(), stream.size(), 0,
                                  address.getSockAddr(), address.getLen());
  if (bytes_received < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return 0;
    }
    throw SocketException("[UdpClient::recvFrom]", strerror(errno));
  }
  stream.resize(static_cast<ssize_t>(bytes_received));
  return bytes_received;
}
