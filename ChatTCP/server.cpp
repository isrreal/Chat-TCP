#include <iostream>
#include <cstring>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 60000

void sendMessages(int sock) {
    std::string message;
    while (true) {
        std::getline(std::cin, message);
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

    obj_server = socket(AF_INET, SOCK_STREAM, 0);

    if (obj_server == 0) {
        perror("Opening of Socket Failed !");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(obj_server, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted))) {
        perror("Can't set the socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(obj_server, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Binding of socket failed !");
        exit(EXIT_FAILURE);
    }

    if (listen(obj_server, 3) < 0) {
        perror("Can't listen from the server !");
        exit(EXIT_FAILURE);
    }

    if ((sock = accept(obj_server, (struct sockaddr*)&address, (socklen_t*)&address_length)) < 0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    std::cout << "Conexão estabelecida com " << address.sin_addr.s_addr << "!" << std::endl;

    std::thread senderThread(sendMessages, sock);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        reader = read(sock, buffer, 1024);
        std::cout << buffer << std::endl;
    }

    senderThread.join();

    return 0;
}
