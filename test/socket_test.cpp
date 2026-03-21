#include "networking/socket.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

using namespace testing;
using namespace net;
using net::Protocol;
using net::SocketType;

class SocketTest : public Test {
 public:
  std::string local_ip = "127.0.0.1";
  std::string test_port = "8080";
  std::string ip_and_port = local_ip + ":" + test_port;
  EndpointAddress test_address;
};

TEST_F(SocketTest, GivenInvalidInputWhenConstructsTCPThenItThrows) {
  ASSERT_THROW(EndpointAddress a(EndpointAddress("", "", TYPE_TCP)),
               SocketException);
}

TEST_F(SocketTest, GivenInvalidInputWhenConstructsUDPThenItThrows) {
  ASSERT_THROW(EndpointAddress a(EndpointAddress("", "", TYPE_UDP)),
               SocketException);
}

TEST_F(SocketTest, GivenValidInputWhenConstructsTCPThenSocketIsCreated) {
  test_address = EndpointAddress(local_ip, test_port, TYPE_TCP);
  net::Socket s(test_address, TYPE_TCP, PROT_TCP);
  EXPECT_THAT(s.getHandle(), Ne(-1));
  EXPECT_THAT(s.getType(), Eq(SOCK_STREAM));
  EXPECT_THAT(s.getFamily(), Eq(AF_INET6));
  EXPECT_THAT(s.getProtocol(), Eq(PROT_TCP));
  EXPECT_THAT(s.getAddress().toString(), Eq(ip_and_port));
}

TEST_F(SocketTest, GivenValidInputWhenConstructsUDPThenSocketIsCreated) {
  test_address = EndpointAddress(local_ip, test_port, TYPE_UDP);
  net::Socket s(test_address, TYPE_UDP, PROT_UDP);
  EXPECT_THAT(s.getHandle(), Ne(-1));
  EXPECT_THAT(s.getType(), Eq(SOCK_DGRAM));
  EXPECT_THAT(s.getFamily(), Eq(AF_INET6));
  EXPECT_THAT(s.getProtocol(), Eq(PROT_UDP));
  EXPECT_THAT(s.getAddress().toString(), Eq(ip_and_port));
}