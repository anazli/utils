#include "networking/socket.h"

#include <unistd.h>

#include <cstring>

/***************************************************************
 *
 * SocketException
 *
 ***************************************************************/

net::SocketException::SocketException(const std::string& context,
                                      const std::string& error)
    : std::runtime_error("Socket Error: "), m_context(context) {
  m_context += error;
}

const char* net::SocketException::what() const noexcept {
  return m_context.c_str();
}

/***************************************************************
 *
 * DataPacket
 *
 ***************************************************************/

net::DataStream::DataStream(size_t size) : m_buffer(size) {}

net::DataStream::DataStream(const uint8_t* data, size_t len) {
  m_buffer.insert(m_buffer.end(), data, data + len);
}

net::DataStream::DataStream(std::initializer_list<uint8_t> list)
    : m_buffer(list) {}

net::DataStream& net::DataStream::append(const uint8_t* data, size_t len) {
  m_buffer.insert(m_buffer.end(), data, data + len);
  return *this;
}

net::DataStream& net::DataStream::append(const std::string& data) {
  const uint8_t* raw_data = reinterpret_cast<const uint8_t*>(data.data());
  append(raw_data, data.size());
  return *this;
}

const uint8_t* net::DataStream::data() const { return m_buffer.data(); }

uint8_t* net::DataStream::data() { return m_buffer.data(); }

size_t net::DataStream::size() const { return m_buffer.size(); }

void net::DataStream::clear() { m_buffer.clear(); }

void net::DataStream::resize(size_t size) { m_buffer.resize(size); }

std::string net::DataStream::toString() const {
  auto d = data();
  auto s = size();
  return std::string(reinterpret_cast<const char*>(d), s);
}

/***************************************************************
 *
 * TcpSocket
 *
 ***************************************************************/

net::TcpSocket::TcpSocket(const std::string& ip, const std::string& port,
                          SocketType type)
    : m_len(sizeof(m_storage)),
      m_family(AF_INET6),
      m_type(type),
      m_protocol(6) {
  std::memset(&m_storage, 0, m_len);

  addrinfo hints;
  std::memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = m_family;
  hints.ai_socktype = m_type;
  hints.ai_protocol = m_protocol;
  hints.ai_flags = AI_PASSIVE | AI_V4MAPPED | AI_ALL;  // accept any connections

  auto context = std::string("[TcpSocket::TcpSocket] ");

  addrinfo* address_info;

  if (auto result =
          getaddrinfo(ip.c_str(), port.c_str(), &hints, &address_info);
      result != 0) {
    throw SocketException(context, gai_strerror(result));
  }

  std::memcpy(&m_storage, address_info->ai_addr, address_info->ai_addrlen);
  freeaddrinfo(address_info);

  if (m_socket_fd = socket(m_family, m_type, m_protocol); m_socket_fd == -1) {
    throw SocketException(context, strerror(errno));
  }

  configureDualStack();
}

net::TcpSocket::TcpSocket(TcpSocket&& other) noexcept
    : m_storage(other.m_storage),
      m_len(other.m_len),
      m_socket_fd(other.m_socket_fd),
      m_family(other.m_family),
      m_type(other.m_type),
      m_protocol(other.m_protocol) {
  other.m_socket_fd = -1;
}

net::TcpSocket& net::TcpSocket::operator=(TcpSocket&& other) noexcept {
  if (this != &other) {
    if (m_socket_fd != -1) ::close(m_socket_fd);

    m_storage = other.m_storage;
    m_len = other.m_len;
    m_socket_fd = other.m_socket_fd;
    m_family = other.m_family;
    m_type = other.m_type;
    m_protocol = other.m_protocol;

    other.m_socket_fd = -1;
  }
  return *this;
}

net::TcpSocket::~TcpSocket() {
  if (m_socket_fd != -1) close(m_socket_fd);
}

int net::TcpSocket::getHandle() const { return m_socket_fd; }

int net::TcpSocket::getType() const { return m_type; }

int net::TcpSocket::getFamily() const { return m_family; }

int net::TcpSocket::getProtocol() const { return m_protocol; }

net::TcpSocket::TcpSocket(int existing_fd, sockaddr_storage addr, socklen_t len)
    : m_socket_fd(existing_fd),
      m_storage(addr),
      m_len(len),
      m_family(addr.ss_family) {
  int type;
  socklen_t type_len = sizeof(type);
  getsockopt(m_socket_fd, SOL_SOCKET, SO_TYPE, &type, &type_len);
  m_type = type;

  int protocol;
  socklen_t prot_len = sizeof(protocol);
  getsockopt(m_socket_fd, SOL_SOCKET, SO_PROTOCOL, &protocol, &prot_len);
  m_protocol = protocol;
}

void net::TcpSocket::configureDualStack() {
  auto context = std::string("[TcpSocket::configureDualStack] ");
  int option{0};  // removes the IPV6_V6ONLY option for the socket
  if (setsockopt(m_socket_fd, IPPROTO_IPV6, IPV6_V6ONLY,
                 static_cast<void*>(&option), sizeof(option)) == -1) {
    throw SocketException(context, "Failed to configure dual stack!");
  }
}