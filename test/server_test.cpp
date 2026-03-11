#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "networking/tcp_server.h"

using namespace testing;

class TcpServerTest : public Test {
 public:
  static constexpr auto local_ip = std::string_view("127.0.0.1");
  static constexpr auto test_port = std::string_view("8080");
};

TEST_F(TcpServerTest, GivenValidInputWhenCreatedThenItIsValid) {
  ASSERT_NO_THROW(net::TcpServer s(local_ip.data(), test_port.data()));
}

TEST_F(TcpServerTest,
       GivenValidSocketWhenItBindsToALocalAddressThenItDoesNoThrow) {
  net::TcpServer s(local_ip.data(), test_port.data());
  auto new_server = std::move(s);

  EXPECT_THAT(s.getHandle(), testing::Eq(-1));
  ASSERT_NO_THROW(new_server.bind());
}