#include "networking/tcp_server.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <future>
#include <memory>
#include <thread>

using namespace testing;
using namespace net;

class TcpServerTest : public Test {
 public:
  std::string local_test_ip = std::string("127.0.0.1");
  std::string local_test_port = std::string("8080");
};

TEST_F(TcpServerTest, GivenValidInputWhenCreatedThenItIsValid) {
  ASSERT_NO_THROW(TcpServer s(local_test_ip, local_test_port));
}

TEST_F(TcpServerTest,
       GivenValidSocketWhenItBindsToALocalAddressThenItDoesntThrow) {
  TcpServer s(local_test_ip, local_test_port);
  auto new_server = std::move(s);

  EXPECT_THAT(s.getHandle(), testing::Eq(-1));
  ASSERT_NO_THROW(new_server.bind());
}

TEST_F(TcpServerTest,
       GivenValidServerWhenItAcceptsThenClientServerConnectionIsCorrect) {
  std::string server_msg("Hello Client! You sent me:");
  std::string client_msg("Howdy, TCP Server!");

  TcpServer server(local_test_ip, local_test_port);
  server.bind();
  server.listen(1);

  TcpClient accepted_peer;
  std::thread t([&accepted_peer, &server, &server_msg] {
    try {
      net::DataStream received(1024);
      accepted_peer = server.accept();
      accepted_peer.recv(received);

      net::DataStream msg_to_send;
      msg_to_send.append(server_msg).append(received.data(), received.size());
      accepted_peer.send(msg_to_send);

    } catch (const net::SocketException& e) {
    }
  });

  TcpClient remote_client;
  remote_client.connect(server.getLocalAddress());

  DataStream client_packet;
  client_packet.append(client_msg);
  remote_client.send(client_packet);

  DataStream result_msg(1024);
  remote_client.recv(result_msg);

  t.join();

  EXPECT_THAT(remote_client.getFamily(), Eq(accepted_peer.getFamily()));
  EXPECT_THAT(remote_client.getType(), Eq(accepted_peer.getType()));
  EXPECT_THAT(remote_client.getProtocol(), Eq(accepted_peer.getProtocol()));
  EXPECT_THAT(result_msg.toString(), Eq(server_msg + client_msg));
  EXPECT_THAT(remote_client.getRemoteAddress().toString(),
              Eq(server.getLocalAddress().toString()));
}