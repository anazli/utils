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
 * EndpointAddress
 *
 ***************************************************************/

net::EndpointAddress::EndpointAddress() : m_storage_len(sizeof(m_storage)) {
  std::memset(&m_storage, 0, m_storage_len);
}

net::EndpointAddress::EndpointAddress(const std::string& ip,
                                      const std::string& port, SocketType type)
    : m_sock_type(type) {
  addrinfo hints, *result;
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = type;
  hints.ai_flags = AI_PASSIVE | AI_V4MAPPED | AI_ALL;
  hints.ai_protocol =
      type == SocketType::TYPE_TCP ? Protocol::PROT_TCP : Protocol::PROT_UDP;

  if (auto r = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result); r != 0) {
    throw SocketException("[EndpointAddress::EndpointAddress]",
                          gai_strerror(r));
  }

  m_storage_len = result->ai_addrlen;
  std::memcpy(&m_storage, result->ai_addr, m_storage_len);
  freeaddrinfo(result);
}

const sockaddr* net::EndpointAddress::getSockAddr() const {
  return reinterpret_cast<const sockaddr*>(&m_storage);
}

sockaddr* net::EndpointAddress::getSockAddr() {
  return reinterpret_cast<sockaddr*>(&m_storage);
}

const socklen_t* net::EndpointAddress::getLen() const { return &m_storage_len; }

socklen_t* net::EndpointAddress::getLen() { return &m_storage_len; }

net::SocketType net::EndpointAddress::getSockType() const {
  return m_sock_type;
}

std::string net::EndpointAddress::toString() const {
  char host[NI_MAXHOST];
  char service[NI_MAXSERV];
  if (auto result =
          getnameinfo(reinterpret_cast<const sockaddr*>(&m_storage),
                      m_storage_len, host, sizeof(host), service,
                      sizeof(service), NI_NUMERICHOST | NI_NUMERICSERV);
      result != 0) {
    return "Unknown Address";
  }

  return std::string(host) + ":" + std::string(service);
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

net::Socket::Socket(const EndpointAddress& address, SocketType type,
                    Protocol protocol)
    : m_local_address(address),
      m_family(AF_INET6),
      m_type(type),
      m_protocol(protocol) {
  if (m_socket_fd = socket(m_family, m_type, m_protocol); m_socket_fd == -1) {
    throw SocketException("[TcpSocket::TcpSocket]", strerror(errno));
  }
  configureDualStack();
}

net::Socket::Socket(SocketType type, Protocol protocol)
    : m_family(AF_INET6), m_type(type), m_protocol(protocol) {
  if (m_socket_fd = socket(m_family, m_type, m_protocol); m_socket_fd == -1) {
    throw SocketException("[TcpSocket::TcpSocket]", strerror(errno));
  }
  configureDualStack();
}

net::Socket::Socket(Socket&& other) noexcept
    : m_local_address(std::move(other.m_local_address)),
      m_remote_address(std::move(other.m_remote_address)),
      m_socket_fd(other.m_socket_fd),
      m_family(other.m_family),
      m_type(other.m_type),
      m_protocol(other.m_protocol) {
  other.m_socket_fd = -1;
}

net::Socket& net::Socket::operator=(Socket&& other) noexcept {
  if (this != &other) {
    if (m_socket_fd != -1) ::close(m_socket_fd);

    m_local_address = std::move(other.m_local_address);
    m_remote_address = std::move(other.m_remote_address);
    m_socket_fd = other.m_socket_fd;
    m_family = other.m_family;
    m_type = other.m_type;
    m_protocol = other.m_protocol;

    other.m_socket_fd = -1;
  }
  return *this;
}

net::Socket::~Socket() {
  if (m_socket_fd != -1) close(m_socket_fd);
}

int net::Socket::getHandle() const { return m_socket_fd; }

int net::Socket::getType() const { return m_type; }

int net::Socket::getFamily() const { return m_family; }

int net::Socket::getProtocol() const { return m_protocol; }

net::EndpointAddress& net::Socket::getLocalAddress() { return m_local_address; }

const net::EndpointAddress& net::Socket::getLocalAddress() const {
  return m_local_address;
}

net::EndpointAddress& net::Socket::getRemoteAddress() {
  return m_remote_address;
}

const net::EndpointAddress& net::Socket::getRemoteAddress() const {
  return m_remote_address;
}

net::Socket::Socket(int existing_fd, sockaddr_storage addr, socklen_t len)
    : m_socket_fd(existing_fd), m_family(addr.ss_family) {
  int type;
  socklen_t type_len = sizeof(type);
  getsockopt(m_socket_fd, SOL_SOCKET, SO_TYPE, &type, &type_len);
  m_type = type;

  int protocol;
  socklen_t prot_len = sizeof(protocol);
  getsockopt(m_socket_fd, SOL_SOCKET, SO_PROTOCOL, &protocol, &prot_len);
  m_protocol = protocol;
}

void net::Socket::configureDualStack() {
  auto context = std::string("[TcpSocket::configureDualStack] ");
  int option{0};  // removes the IPV6_V6ONLY option for the socket
  if (setsockopt(m_socket_fd, IPPROTO_IPV6, IPV6_V6ONLY,
                 static_cast<void*>(&option), sizeof(option)) == -1) {
    throw SocketException(context, "Failed to configure dual stack!");
  }
}