#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include "bank.h"

class BankServer {
private:
    int port;
    Bank bank;
    std::vector<std::thread> clientThreads;
    std::atomic<bool> running;

    void handleClient(int clientSocket);

public:
    BankServer(int port, const std::string& usersFilePath);
    ~BankServer();

    void start();
    void stop();
};