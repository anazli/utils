#include "networking/socket.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

using namespace testing;
using namespace net;

class SocketTest : public Test {
 public:
  static constexpr auto remote_ip = std::string_view("www.example.com");
  static constexpr auto local_ip = std::string_view("127.0.0.1");
  static constexpr auto test_port = std::string_view("8080");
};

TEST_F(SocketTest, GivenInvalidInputWhenConstructsTCPThenItThrows) {
  ASSERT_THROW(EndpointAddress a(
                   EndpointAddress("", "", net::EndpointAddress::TYPE_TCP)),
               SocketException);
}

TEST_F(SocketTest, GivenInvalidInputWhenConstructsUDPThenItThrows) {
  ASSERT_THROW(EndpointAddress a(
                   EndpointAddress("", "", net::EndpointAddress::TYPE_UDP)),
               SocketException);
}

TEST_F(SocketTest, GivenValidInputWhenConstructsTCPThenSocketIsCreated) {
  net::Socket s(EndpointAddress::TYPE_TCP, EndpointAddress::PROT_TCP);
  EXPECT_THAT(s.getHandle(), Ne(-1));
  EXPECT_THAT(s.getType(), Eq(SOCK_STREAM));
  EXPECT_THAT(s.getFamily(), Eq(AF_INET6));
  EXPECT_THAT(s.getProtocol(), Eq(net::EndpointAddress::PROT_TCP));
}

TEST_F(SocketTest, GivenValidInputWhenConstructsUDPThenSocketIsCreated) {
  net::Socket s(EndpointAddress::TYPE_UDP, EndpointAddress::PROT_UDP);
  EXPECT_THAT(s.getHandle(), Ne(-1));
  EXPECT_THAT(s.getType(), Eq(SOCK_DGRAM));
  EXPECT_THAT(s.getFamily(), Eq(AF_INET6));
  EXPECT_THAT(s.getProtocol(), Eq(net::EndpointAddress::PROT_UDP));
}