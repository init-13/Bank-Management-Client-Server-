#pragma once
#include <string>

class BankClient {
private:
    int serverSocket;

public:
    BankClient(int port);
    ~BankClient();

    bool connectToServer();
    void disconnectFromServer();
    std::string sendRequest(const std::string& request);
};

