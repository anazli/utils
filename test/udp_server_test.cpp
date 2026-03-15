#include "networking/udp_server.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <future>
#include <memory>
#include <thread>

#include "networking/udp_client.h"

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
       GivenValidSocketWhenItBindsToALocalAddressThenItDoesntThrow) {
  net::UdpServer s(local_ip.data(), test_port.data());
  auto new_server = std::move(s);

  EXPECT_THAT(s.getHandle(), testing::Eq(-1));
  ASSERT_NO_THROW(new_server.bind());
}

TEST_F(UdpServerTest,
       GivenValidServerWhenItBindsThenClientServerConnectionIsCorrect) {
  std::string server_msg("Hello Client! You sent me:");
  std::string client_msg("Howdy, UDP Server!");

  net::UdpServer server(local_ip.data(), test_port.data());
  server.bind();

  auto remote_client = net::UdpClient(local_ip.data(), test_port.data());

  std::thread t([&server, &remote_client, &server_msg] {
    try {
      net::DataStream received;
      server.recvFrom(received, remote_client.getAddress(), 1024);

      net::DataStream msg_to_send;
      msg_to_send.append(server_msg).append(received.data(), received.size());
      server.sendTo(msg_to_send, remote_client.getAddress());

    } catch (const net::SocketException& e) {
    }
  });

  net::DataStream client_packet;
  client_packet.append(client_msg);
  remote_client.sendTo(client_packet, server.getAddress());

  net::DataStream result_msg;
  remote_client.recvFrom(result_msg, server.getAddress(), 1024);

  t.join();

  EXPECT_THAT(remote_client.getFamily(), Eq(server.getFamily()));
  EXPECT_THAT(remote_client.getType(), Eq(server.getType()));
  EXPECT_THAT(remote_client.getProtocol(), Eq(server.getProtocol()));
  EXPECT_THAT(result_msg.toString(), Eq(server_msg + client_msg));
}