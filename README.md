# Utils - Networking Library

A collection of utility entities that I can use in other projects. So far only networking components are included like TCP/UDP Server/Client implementations for a Linux environment. More to be added in the future...

The repo contains also a playground code in src/main that implements a chat Server which can handle multiple client connections.

## Project Structure

```
├── include/networking/          # Public API headers
│   ├── socket.h                # Base Socket class
│   ├── tcp_client.h            # TCP client implementation
│   ├── tcp_server.h            # TCP server with epoll support
│   ├── udp_client.h            # UDP client implementation
│   └── udp_server.h            # UDP server implementation
├── src/
│   ├── networking/             # Implementation files
│   │   ├── socket.cpp
│   │   ├── tcp_client.cpp
│   │   ├── tcp_server.cpp
│   │   ├── udp_client.cpp
│   │   └── udp_server.cpp
│   └── main/                   # Chat server/client playground apps
│       ├── server.cpp
│       └── client.cpp
└── test/                       # Unit tests
    ├── socket_test.cpp
    ├── tcp_client_test.cpp
    ├── tcp_server_test.cpp
    ├── udp_client_test.cpp
    └── udp_server_test.cpp
```

## Building
```bash
mkdir build && cd build
cmake .. -DENABLE_TESTING=ON
cmake --build .
```

## Exception Handling

All network operations throw `SocketException` on errors:

```cpp
try {
  net::TcpClient client;
  client.connect(server_addr);
} catch (const net::SocketException& e) {
  std::cerr << e.what() << std::endl;
}
```

## Chat Server/Client Example

The repository includes a playground chat application with a TCP multi-client broadcast server:

### Start Server
```bash
./build/server
```

Server listens on `127.0.0.1:8080` and broadcasts messages from all connected clients to each other.

### Connect Client
```bash
./build/client
```

Enter messages at the prompt. Messages are sent to the server and broadcasted to all other connected clients.

## Testing

Run the test suite:

```bash
cd build
ctest
```
