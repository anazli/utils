#include "networking/tcp_client.h"

#include <cstring>

net::TcpClient::TcpClient(const std::string& host, const std::string& port)
    : Socket(EndpointAddress::TYPE_TCP, EndpointAddress::PROT_TCP),
      m_address(host, port, EndpointAddress::TYPE_TCP) {}

net::TcpClient::TcpClient(int existing_fd, sockaddr_storage addr, socklen_t len)
    : Socket(existing_fd, addr, len) {}

void net::TcpClient::connect() {
  if (::connect(m_socket_fd,
                reinterpret_cast<sockaddr*>(m_address.getSockAddr()),
                *m_address.getLen()) == -1) {
    throw SocketException("[TcpClient::connect]", strerror(errno));
  }
}

ssize_t net::TcpClient::send(const DataStream& message, int flags) {
  auto bytes_sent = ::send(m_socket_fd, message.data(), message.size(), flags);
  if (bytes_sent == -1) {
    if (errno == EINTR) {
      return 0;
    }
    throw SocketException("[TcpClient::send]", strerror(errno));
  }
  return bytes_sent;
}

ssize_t net::TcpClient::recv(DataStream& message, int flags) {
  std::string context("[TcpClient::recv]");
  if (message.size() == 0) {
    throw SocketException(context, "message size buffer is empty");
  }
  auto bytes_received =
      ::recv(m_socket_fd, message.data(), message.size(), flags);

  if (bytes_received > 0) {
    message.resize(bytes_received);
  } else if (bytes_received == 0) {
    message.clear();
    return 0;
  } else {
    throw SocketException(context, strerror(errno));
  }
  return bytes_received;
}