// BankClient.cpp
#include "BankClient.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

BankClient::BankClient(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Assuming server is running on localhost
    serverAddr.sin_port = htons(port);

    if (connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        close(serverSocket);
        serverSocket = -1;
    }
}

BankClient::~BankClient() {
    disconnectFromServer();
}

bool BankClient::connectToServer() {
    return serverSocket != -1;
}

void BankClient::disconnectFromServer() {
    if (serverSocket != -1) {
        close(serverSocket);
        serverSocket = -1;
    }
}

std::string BankClient::sendRequest(const std::string& request) {
    std::string response;

    if (serverSocket == -1) {
        std::cerr << "Not connected to the server" << std::endl;
        return response;
    }

    if (send(serverSocket, request.c_str(), request.size(), 0) < 0) {
        std::cerr << "Error sending request" << std::endl;
        return response;
    }

    char buffer[4096] = { 0 };
    if (recv(serverSocket, buffer, sizeof(buffer), 0) < 0) {
        std::cerr << "Error receiving response" << std::endl;
        return response;
    }

    response = buffer;
    return response;
}
