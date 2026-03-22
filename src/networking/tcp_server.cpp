#include "networking/tcp_server.h"

#include <sys/epoll.h>
#include <unistd.h>

#include <cstring>
#include <print>

namespace {

bool shouldExit(const std::string& message) {
  return message == "exit" || message == "quit";
}

}  // namespace

net::TcpServer::TcpServer(const std::string& local_address,
                          const std::string& port)
    : Socket(EndpointAddress(local_address, port), SocketType::TYPE_TCP,
             Protocol::PROT_TCP) {}

net::TcpServer::TcpServer(const EndpointAddress& address)
    : Socket(address, SocketType::TYPE_TCP, Protocol::PROT_TCP) {}

void net::TcpServer::bind() {
  int opt = 1;  // prevent address already in use after restart
  setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (::bind(m_socket_fd,
             reinterpret_cast<sockaddr*>(m_local_address.getSockAddr()),
             *m_local_address.getLen()) == -1) {
    throw SocketException("[TcpServer::bind]", strerror(errno));
  }
}

void net::TcpServer::listen(int backlog) {
  if (::listen(m_socket_fd, backlog) == -1) {
    throw SocketException("[TcpServer::listen]", strerror(errno));
  }
}

net::TcpClient net::TcpServer::accept() {
  // blocking call
  int new_client = ::accept(m_socket_fd, m_remote_address.getSockAddr(),
                            m_remote_address.getLen());

  return TcpClient(new_client, m_remote_address);
}

void net::TcpServer::listenAndServe() {
  auto context = std::string("[TcpServer::listenAndServe]");
  int epoll_fd;
  epoll_event ev, events[SOMAXCONN];
  bind();
  std::println("Server is listening on port:{}", m_local_address.getPort());
  listen();

  epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    throw SocketException(context, strerror(errno));
  }

  ev.events = EPOLLIN;
  ev.data.fd = getHandle();

  // adds new fd (server) to data structure
  if (auto r = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, getHandle(), &ev); r == -1) {
    throw SocketException(context, strerror(errno));
  }

  while (true) {
    auto nfds = epoll_wait(epoll_fd, events, SOMAXCONN, -1);
    if (nfds == -1) {
      throw SocketException(context, strerror(errno));
    }

    for (auto conn = 0; conn < nfds; ++conn) {
      // if event is from server add new client
      if (events[conn].data.fd == getHandle()) {
        handleNewConnection(epoll_fd, ev);
      }  // if event is from client read input and respond
      else {
        handleClientEvent(epoll_fd, events[conn]);
      }
    }
  }
}

void net::TcpServer::handleNewConnection(int epoll_fd, epoll_event& event) {
  std::println("Accepting new connection");
  auto client = ::accept(m_socket_fd, m_remote_address.getSockAddr(),
                         m_remote_address.getLen());
  std::println("Client: {} connected..", m_remote_address.toString());

  event.events = EPOLLIN | EPOLLET;
  event.data.fd = client;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event) == -1) {
    throw SocketException("[TcpServer::handleNewConnection]", strerror(errno));
  }

  m_connected_clients[client] = m_remote_address;
}

void net::TcpServer::handleClientEvent(int epoll_fd, epoll_event& event) {
  net::DataStream received_msg(1024);
  int bytes_received =
      ::recv(event.data.fd, received_msg.data(), received_msg.size(), 0);
  if (bytes_received < 1) {
    if (bytes_received < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      return;
    }

    if (auto r = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event.data.fd, &event);
        r == -1) {
      throw SocketException("[TcpServer::handleClientEvent]", strerror(errno));
    }
    ::close(event.data.fd);
    m_connected_clients.erase(event.data.fd);
    return;
  }
  received_msg.resize(bytes_received);

  std::println("Server received the following message: {}",
               received_msg.toString());

  net::DataStream msg_to_send;
  msg_to_send.append(m_connected_clients[event.data.fd].toString() + ":")
      .append(received_msg.data(), received_msg.size());

  broadcast(event.data.fd, msg_to_send);
}

void net::TcpServer::broadcast(int sender_fd, const net::DataStream& message) {
  for (auto& [client, address] : m_connected_clients) {
    if (client == sender_fd) continue;
    if (auto bytes_sent = ::send(client, message.data(), message.size(), 0);
        bytes_sent < 1) {
      throw SocketException("[TcpServer::handleClientEvent]", strerror(errno));
    }
  }
}