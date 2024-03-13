#include <iostream>
#include <cstring>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
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
    std::string mensagem = "";
    int sock = 0; 
    int reader = 1;
    struct sockaddr_in enderecoDoServer;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Erro na criação do socket!" << std::endl;
        return -1;
    }

    enderecoDoServer.sin_family = AF_INET;
    enderecoDoServer.sin_port = htons(PORT);

    // Convertendo endereços IPv4 e IPv6 de texto para sua forma binária

    if(inet_pton(AF_INET, "127.0.0.1", &enderecoDoServer.sin_addr) <= 0) {
        std::cout << "Endereço inválido! Este IP não é suportado!" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*) &enderecoDoServer, sizeof(enderecoDoServer)) < 0) {
        std::cout << "Falha de conexão: Não foi possível estabelecer uma conexão sobre este socket!" << std::endl;
        return -1;
    }
    
    std::thread senderThread(sendMessages, sock);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        reader = read(sock, buffer, 1024);
        std::cout << buffer << std::endl;
    }

    senderThread.join();

    return 0;
}
