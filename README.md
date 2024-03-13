# Chat-TCP

Chat-TCP is a simple chat client-server application based on TCP communication protocol.

## Resetting the Chat (Linux):

If you are using Linux and want to reset the chat, you can execute the following command in the terminal:

```bash
kill -9 $(lsof -i :60000) && clear
```

This command will terminate any processes using port 60000 and clear the terminal screen.

## Steps for Running the Program:

### 1. Compile and Execute Server:
Compile the server source code using the following command:

```bash
g++ server.cpp -o server -std=c++17 && ./server
```

This will compile and execute the server program.

### 2. Compile and Execute Client:
Compile the client source code using the following command:

```bash
g++ client.cpp -o client -std=c++17 && ./client
```

This will compile and execute the client program.

### 3. Send Messages in Terminal:
Once the server and client are running, you can send messages through the terminal interface provided by the client program.

Enjoy chatting!
