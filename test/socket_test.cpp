#include "networking/socket.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

class SocketTest : public testing::Test {};

TEST_F(SocketTest, createsATcpClient) {
  std::unique_ptr<net::Socket> s = std::make_unique<net::TcpClient>("", 0);
  ASSERT_THAT(s->getHandle(), testing::Ne(-1));
  ASSERT_THAT(s->getType(), testing::Ne(SOCK_STREAM));
  ASSERT_THAT(s->getHandle(), testing::Ne(AF_INET));
}
