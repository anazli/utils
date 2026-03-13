#include "networking/socket.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

using namespace testing;

class SocketTest : public Test {
 public:
  static constexpr auto remote_ip = std::string_view("www.example.com");
  static constexpr auto local_ip = std::string_view("127.0.0.1");
  static constexpr auto test_port = std::string_view("8080");
};

TEST_F(SocketTest, GivenInvalidInputWhenConstructsTCPThenItThrows) {
  ASSERT_THROW(net::Socket s("", "", net::Socket::TYPE_TCP),
               net::SocketException);
}

TEST_F(SocketTest, GivenInvalidInputWhenConstructsUDPThenItThrows) {
  ASSERT_THROW(net::Socket s("", "", net::Socket::TYPE_UDP),
               net::SocketException);
}

TEST_F(SocketTest, GivenValidInputWhenConstructsTCPThenSocketIsCreated) {
  net::Socket s(remote_ip.data(), test_port.data(), net::Socket::TYPE_TCP);
  EXPECT_THAT(s.getHandle(), Ne(-1));
  EXPECT_THAT(s.getType(), Eq(SOCK_STREAM));
  EXPECT_THAT(s.getFamily(), Eq(AF_INET6));
  EXPECT_THAT(s.getProtocol(), Eq(net::Socket::PROT_TCP));
}

TEST_F(SocketTest, GivenValidInputWhenConstructsUDPThenSocketIsCreated) {
  net::Socket s(remote_ip.data(), test_port.data(), net::Socket::TYPE_UDP);
  EXPECT_THAT(s.getHandle(), Ne(-1));
  EXPECT_THAT(s.getType(), Eq(SOCK_DGRAM));
  EXPECT_THAT(s.getFamily(), Eq(AF_INET6));
  EXPECT_THAT(s.getProtocol(), Eq(net::Socket::PROT_UDP));
}