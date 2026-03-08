#include "networking/socket.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

using namespace testing;

class SocketTest : public Test {};

TEST_F(SocketTest, GivenInvalidInputWhenConstructsThenItThrows) {
  ASSERT_THROW(net::TcpSocket s("", ""), net::SocketException);
}

TEST_F(SocketTest, GivenValidInputWhenConstructsThenSocketIsCreated) {
  net::TcpSocket s("www.example.com", "80");
  EXPECT_THAT(s.getHandle(), Ne(-1));
  EXPECT_THAT(s.getType(), Eq(SOCK_STREAM));
  EXPECT_THAT(s.getFamily(), Eq(AF_INET));
}

TEST_F(SocketTest, GivenValidSocketWhenBindsToAddressThenItDoesNotThrow) {
  net::TcpSocket s("127.0.0.1", "8080");
  ASSERT_NO_THROW(s.bind());
}