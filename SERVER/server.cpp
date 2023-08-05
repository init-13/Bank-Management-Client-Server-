
#include "BankServer.h"
#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

BankServer::BankServer(int port, const std::string& usersFilePath) : port(port), bank(usersFilePath), running(false) {}

BankServer::~BankServer() {
    stop();
}

void BankServer::handleClient(int clientSocket) {

    std::cout << "A Client connected." << std::endl;
     
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE] = {0};

    while(1){
    // Receive data from the client
    int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesRead <= 0) {
        std::cerr << "Error receiving data from the client." << std::endl;
        close(clientSocket);
        return;
    }

    // Parse the received message (assuming it's a string)
    std::string request(buffer);
    std::cout<< "Client requested: "<<buffer<<std::endl;

    // Process the client's request
    std::string response;
    if (request.find("LOGIN|") == 0) {
        // Extract username and password from the request
        std::string username, password;
        size_t delimiterPos = request.find("|");
        if (delimiterPos != std::string::npos) {
            username = request.substr(delimiterPos + 1);
            delimiterPos = username.find("|");
            if (delimiterPos != std::string::npos) {
                password = username.substr(delimiterPos + 1);
                username = username.substr(0, delimiterPos);
            }
        }

        // Call the login function of the Bank class to verify credentials
        User* user = bank.login(username, password);
        if (user != nullptr) {
            // Successful login
            response = "SUCCESS|" + std::to_string(user->getAccountBalance());
        } else {
            // Failed login
            response = "FAILURE|Invalid username or password.";
        }
    } else if (request.find("REGISTER|") == 0) {
        // Extract username and password from the request
        std::string username, password;
        size_t delimiterPos = request.find("|");
        if (delimiterPos != std::string::npos) {
            username = request.substr(delimiterPos + 1);
            delimiterPos = username.find("|");
            if (delimiterPos != std::string::npos) {
                password = username.substr(delimiterPos + 1);
                username = username.substr(0, delimiterPos);
            }
        }
        //    std::cout<<username<<" "<<password<<std::endl;
        // Call the registerUser function of the Bank class to register a new user
        if (bank.registerUser(username, password)) {
            // Successful registration
            response = "SUCCESS|User registered successfully.";
        } else {
            // Failed registration (username already exists)
            response = "FAILURE|Username already taken.";
        }
    } else {
        // Invalid request
        response = "FAILURE|Invalid request.|EXITING";
        break;
    }

    // Send the response back to the client
    int bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
    if (bytesSent <= 0) {
        std::cerr << "Error sending response to the client." << std::endl;
    }
    }
    // Close the client socket
    close(clientSocket);
}

void BankServer::start() {
    if (running) return;
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding" << std::endl;
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Error listening" << std::endl;
        close(serverSocket);
        return;
    }

    running = true;
    std::cout << "Bank server started on port " << port << std::endl;

    while (running) {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

        if (clientSocket < 0) {
            std::cerr << "Error accepting client connection" << std::endl;
            continue;
        }
        
        std::thread clientThread(&BankServer::handleClient, this, clientSocket);
        clientThreads.push_back(std::move(clientThread));
    }

    // Wait for client threads to finish
    for (auto& thread : clientThreads) {
        thread.join();
    }

    close(serverSocket);
}

void BankServer::stop() {
    if (!running) return;

    running = false;
    for (auto& thread : clientThreads) {
        thread.join();
    }

    clientThreads.clear();
}


int main() {
    const int port = 8080; // Change to the desired port number
    const std::string usersFilePath = "database/users.txt"; // Change to the path of your user data file

    BankServer bankServer(port, usersFilePath);

    bankServer.start();

    // Keep the server running until user input
    std::cout << "Bank server is running. Press Enter to stop." << std::endl;
    std::cin.get();

    bankServer.stop();

    std::cout << "Bank server stopped." << std::endl;

    return 0;
}

