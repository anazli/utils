#include "networking/tcp_server.h"

#include <sys/epoll.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

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
  std::cout << "Server is listening..." << std::endl;
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
        net::DataStream received_msg(1024);
        handleClientEvent(epoll_fd, received_msg, events[conn]);
      }
    }
  }
}

void net::TcpServer::handleNewConnection(int epoll_fd, epoll_event& event) {
  std::cout << "Accepting new connection" << std::endl;
  auto client = ::accept(m_socket_fd, m_remote_address.getSockAddr(),
                         m_remote_address.getLen());
  std::cout << "Client: " << m_remote_address.toString() << " connected.."
            << std::endl;

  event.events = EPOLLIN | EPOLLET;
  event.data.fd = client;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event) == -1) {
    throw SocketException("[TcpServer::handleNewConnection]", strerror(errno));
  }
}

void net::TcpServer::handleClientEvent(int epoll_fd, DataStream& buffer,
                                       epoll_event& event) {
  int bytes_received = ::recv(event.data.fd, buffer.data(), buffer.size(), 0);
  if (bytes_received < 1) {
    if (bytes_received < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      return;
    }

    if (auto r = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event.data.fd, &event);
        r == -1) {
      throw SocketException("[TcpServer::handleClientEvent]", strerror(errno));
    }
    ::close(event.data.fd);
    return;
  }
  buffer.resize(bytes_received);

  std::cout << "Server received the following message: " << buffer.toString()
            << std::endl;

  net::DataStream msg_to_send;
  msg_to_send.append("Server: ").append(buffer.data(), buffer.size());

  if (auto bytes_sent =
          ::send(event.data.fd, msg_to_send.data(), msg_to_send.size(), 0);
      bytes_sent < 1) {
    throw SocketException("[TcpServer::handleClientEvent]", strerror(errno));
  }
}