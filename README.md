# Utils - Networking Library

A collection of utility entities that I can use in other projects. So far only networking components are included like TCP/UDP Server/Client implementations for a Linux environment. More to be added in the future...

The repo contains also a playground code in src/main that implements a chat Server which can handle multiple client connections.

## Building
```bash
mkdir build && cd build
cmake .. -DENABLE_TESTING=ON
cmake --build .
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
