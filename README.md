# chat
A basic chat C++ server and Python client for programming practice with gRPC

## Overview

This basic chat utility has two components:

1. A chat server written in C++
2. A chat client written in Python

The chat server is run using the command line:

```Server.exe --url="localhost:50051"```

Each chat client is run using the command line:

```python .\client.py --client-id="Alice" --url="localhost:50051"```

The client-id needs to be unique for each user.

This basic chat utility has no authentication.

## Code Structure

The server and client use gRPC to communicate.

To regenerate the gRPC code for the Python client use the command:

```python -m grpc_tools.protoc -I="../Proto" --python_out=. --grpc_python_out=. "../Proto/chat.proto"```

The C++ server is built using CMake, so this will regenerate the gRPC code if required.

There are two fundamental gRPC calls the client can make to the server:

1. `Subscribe`: This registers the client with the server. The server sends back a stream of notifications, so it keeps the notification writer open for as long as the client is connected.

2. `SendMessage`: This is the client sending a chat message to the server. This message will be broadcast to all subscribed clients.