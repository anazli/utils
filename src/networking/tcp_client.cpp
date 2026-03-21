#include "networking/tcp_client.h"

#include <cstring>

net::TcpClient::TcpClient()
    : Socket(SocketType::TYPE_TCP, Protocol::PROT_TCP) {}

net::TcpClient::TcpClient(int existing_fd,
                          const EndpointAddress& remote_address)
    : Socket(existing_fd, remote_address) {}

net::EndpointAddress& net::TcpClient::getLocalAddress() {
  if (::getsockname(m_socket_fd, m_local_address.getSockAddr(),
                    m_local_address.getLen()) == -1) {
    throw SocketException("[TcpClient::getLocalAddress]", strerror(errno));
  }
  return m_local_address;
}

void net::TcpClient::connect(const EndpointAddress& remote_address) {
  m_remote_address = remote_address;
  if (::connect(m_socket_fd,
                reinterpret_cast<sockaddr*>(m_remote_address.getSockAddr()),
                *m_remote_address.getLen()) == -1) {
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