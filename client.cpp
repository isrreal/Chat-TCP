#include <iostream>
#include <cstring>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 60000

// Function to send messages to the server
void sendMessages(int sock) {
    std::string message;
    // Loop to continuously send messages while the program is running
    while (true) {
        // Wait for user input to send a message
        std::getline(std::cin, message);
        // Send the message to the server
        send(sock, message.data(), message.length(), 0);
    }
}

int main(int argc, char const* argv[]) {
    std::string mensagem = "";
    int sock = 0; 
    int reader = 1;
    struct sockaddr_in serverAddress;
    char buffer[1024] = {0};

    // Create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Error creating socket!" << std::endl;
        return -1;
    }

    // Configure the server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        std::cout << "Invalid address! This IP is not supported!" << std::endl;
        return -1;
    }

    // Connect the client to the server
    if (connect(sock, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
        std::cout << "Connection failed: Could not establish a connection on this socket!" << std::endl;
        return -1;
    }
    
    // Start a thread to send messages to the server
    std::thread senderThread(sendMessages, sock);

    // Loop to receive messages from the server
    while (true) {
        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
        // Read data received from the server
        reader = read(sock, buffer, 1024);
        // Display the received message from the server
        std::cout << buffer << std::endl;
    }

    // Wait for the message sending thread to finish
    senderThread.join();

    return 0;
}
