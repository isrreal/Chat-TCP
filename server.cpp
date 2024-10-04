#include <iostream>
#include <cstring>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 60000

// Function that sends messages to the client
void sendMessages(int sock) {
    std::string message;
    // Infinite loop to send messages while the program is running
    while (true) {
        // Wait for user input to send a message
        std::getline(std::cin, message);
        // Send the message to the client
        send(sock, message.c_str(), message.length(), 0);
    }
}

int main(int argc, char const* argv[]) {
    int obj_server = 0;
    int sock = 0;
    int reader = 0;
    struct sockaddr_in address;
    int opted = 1;
    int address_length = sizeof(address);
    char buffer[1024] = {0};

    // Create the socket
    obj_server = socket(AF_INET, SOCK_STREAM, 0);

    // Check if the socket was created successfully
    if (obj_server == 0) {
        perror("Opening of Socket Failed !");
        exit(EXIT_FAILURE);
    }

    // Configure socket options, such as address reuse
    if (setsockopt(obj_server, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted))) {
        perror("Can't set the socket");
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the IP address to the port
    if (bind(obj_server, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Binding of socket failed !");
        exit(EXIT_FAILURE);
    }

    // Enable the server to accept connections, with a queue of up to 3 pending connections
    if (listen(obj_server, 3) < 0) {
        perror("Can't listen from the server !");
        exit(EXIT_FAILURE);
    }

    // Wait for a connection from a client
    if ((sock = accept(obj_server, (struct sockaddr*)&address, (socklen_t*)&address_length)) < 0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    // Inform that a connection was established and show the client's address
    std::cout << "Connection established with " << address.sin_addr.s_addr << "!" << std::endl;

    // Create a thread to send messages to the client
    std::thread senderThread(sendMessages, sock);

    // Loop to receive messages from the client
    while (true) {
        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
        // Read the received data from the client
        reader = read(sock, buffer, 1024);
        // Display the received message from the client
        std::cout << buffer << std::endl;
    }

    // Wait for the message sending thread to finish
    senderThread.join();

    return 0;
}
