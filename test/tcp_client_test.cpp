#include "networking/tcp_client.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

using namespace testing;
using namespace net;
using net::SocketType;

class TcpClientTest : public Test {
 public:
  std::string test_ip = std::string("www.google.com");
  std::string test_port = std::string("80");
  std::string remote_ip_and_port = test_ip + ":" + test_port;
  net::EndpointAddress remote_test_address =
      EndpointAddress(test_ip, test_port);
};

TEST_F(TcpClientTest, GivenValidInputWhenCreatedThenItIsValid) {
  ASSERT_NO_THROW(net::TcpClient c);
}

TEST_F(TcpClientTest,
       GivenValidSocketWhenItConnectsToRemoteAddressThenItDoesNoThrow) {
  net::TcpClient c;

  EXPECT_THAT(c.getHandle(), Ne(-1));
  ASSERT_NO_THROW(c.connect(remote_test_address));
  EXPECT_THAT(c.getRemoteAddress().toString(),
              Eq(remote_test_address.toString()));
}