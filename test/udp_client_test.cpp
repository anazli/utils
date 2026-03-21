#include "networking/udp_client.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

using namespace testing;

class UdpClientTest : public Test {
 public:
};

TEST_F(UdpClientTest, GivenValidInputWhenCreatedThenItIsValid) {
  ASSERT_NO_THROW(net::UdpClient c);
}