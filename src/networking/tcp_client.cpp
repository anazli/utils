#include "networking/tcp_client.h"

#include <cstring>

net::TcpClient::TcpClient(const std::string& host, const std::string& port)
    : TcpSocket(host, port, net::TcpSocket::TCP) {}

net::TcpClient::TcpClient(int existing_fd, sockaddr_storage addr, socklen_t len)
    : TcpSocket(existing_fd, addr, len) {}

void net::TcpClient::connect() {
  if (::connect(m_socket_fd, reinterpret_cast<sockaddr*>(&m_storage), m_len) ==
      -1) {
    throw SocketException("[TcpSocket::bind]", strerror(errno));
  }
}

ssize_t net::TcpClient::send(const DataStream& message, int flags) {
  auto bytes_sent = ::send(m_socket_fd, message.data(), message.size(), flags);
  if (bytes_sent == -1) {
    if (errno == EINTR) {
      return 0;
    }
    throw SocketException("[TcpSocket::send]", strerror(errno));
  }
  return bytes_sent;
}

ssize_t net::TcpClient::recv(DataStream& message, int flags) {
  if (message.size() == 0) {
    throw SocketException("[TcpSocket::recv]", "message size buffer is empty");
  }
  auto bytes_received =
      ::recv(m_socket_fd, message.data(), message.size(), flags);

  if (bytes_received > 0) {
    message.resize(bytes_received);
  } else if (bytes_received == 0) {
    message.clear();
    return 0;
  } else {
    throw SocketException("[TcpSocket::recv]", strerror(errno));
  }
  return bytes_received;
}