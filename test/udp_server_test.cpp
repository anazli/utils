#include "networking/udp_server.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <future>
#include <memory>
#include <thread>

using namespace testing;

class UdpServerTest : public Test {
 public:
  static constexpr auto local_ip = std::string_view("127.0.0.1");
  static constexpr auto test_port = std::string_view("8080");
};

TEST_F(UdpServerTest, GivenValidInputWhenCreatedThenItIsValid) {
  ASSERT_NO_THROW(net::UdpServer s(local_ip.data(), test_port.data()));
}

TEST_F(UdpServerTest,
       GivenValidSocketWhenItBindsToALocalAddressThenItDoesNoThrow) {
  net::UdpServer s(local_ip.data(), test_port.data());
  auto new_server = std::move(s);

  EXPECT_THAT(s.getHandle(), testing::Eq(-1));
  ASSERT_NO_THROW(new_server.bind());
}

/*TEST_F(UdpServerTest,
       GivenValidServerWhenItAcceptsThenClientServerConnectionIsCorrect) {
  std::string server_msg("Hello Client! You sent me:");
  std::string client_msg("Howdy, TCP Server!");

  net::UdpServer server(local_ip.data(), test_port.data());
  server.bind();
  server.listen(1);

  net::TcpClient accepted_peer;
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

  auto remote_client = net::TcpClient(local_ip.data(), test_port.data());
  remote_client.connect();

  net::DataStream client_packet;
  client_packet.append(client_msg);
  remote_client.send(client_packet);

  net::DataStream result_msg(1024);
  remote_client.recv(result_msg);

  t.join();

  EXPECT_THAT(remote_client.getFamily(), Eq(accepted_peer.getFamily()));
  EXPECT_THAT(remote_client.getType(), Eq(accepted_peer.getType()));
  EXPECT_THAT(remote_client.getProtocol(), Eq(accepted_peer.getProtocol()));
  EXPECT_THAT(result_msg.toString(), Eq(server_msg + client_msg));
}*/