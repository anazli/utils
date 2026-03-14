#include "networking/udp_client.h"

net::UdpClient::UdpClient(const std::string& host, const std::string& port)
    : Socket(host, port, SocketType::TYPE_UDP) {}