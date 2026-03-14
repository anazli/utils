#include "networking/udp_client.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

using namespace testing;

class UdpClientTest : public Test {
 public:
  static constexpr auto remote_ip = std::string_view("www.example.com");
  static constexpr auto local_ip = std::string_view("127.0.0.1");
  static constexpr auto test_port = std::string_view("8080");
};

TEST_F(UdpClientTest, GivenValidInputWhenCreatedThenItIsValid) {
  ASSERT_NO_THROW(net::UdpClient c(remote_ip.data(), test_port.data()));
}