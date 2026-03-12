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

TEST_F(SocketTest, GivenInvalidInputWhenConstructsThenItThrows) {
  ASSERT_THROW(net::Socket s("", "", net::Socket::TCP), net::SocketException);
}

TEST_F(SocketTest, GivenValidInputWhenConstructsThenSocketIsCreated) {
  net::Socket s(remote_ip.data(), test_port.data(), net::Socket::TCP);
  EXPECT_THAT(s.getHandle(), Ne(-1));
  EXPECT_THAT(s.getType(), Eq(SOCK_STREAM));
  EXPECT_THAT(s.getFamily(), Eq(AF_INET6));
  EXPECT_THAT(s.getProtocol(), Eq(6));
}
