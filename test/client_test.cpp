#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "networking/tcp_client.h"

using namespace testing;

class TcpClientTest : public Test {
 public:
  static constexpr auto remote_ip = std::string_view("www.example.com");
  static constexpr auto local_ip = std::string_view("127.0.0.1");
  static constexpr auto test_port = std::string_view("8080");
};

TEST_F(TcpClientTest, GivenValidInputWhenCreatedThenItIsValid) {
  ASSERT_NO_THROW(net::TcpClient c(remote_ip.data(), test_port.data()));
}

TEST_F(TcpClientTest,
       GivenValidSocketWhenItConnectsToAnAddressThenItDoesNoThrow) {
  net::TcpClient c(remote_ip.data(), test_port.data());
  ASSERT_NO_THROW(c.connect());
}