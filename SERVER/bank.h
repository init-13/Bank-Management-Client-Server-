#include <vector>
#include <iostream>
#include <fstream>
#include "user.h"

class Bank {
private:
    std::vector<User> users;
    std::string usersFilePath;

    // Utility function to read user data from file
    void readUsersFromFile();
    // Utility function to write user data to file
    void writeUsersToFile();

public:
    Bank(const std::string& usersFilePath);
    User* login(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password);
};

Bank::Bank(const std::string& usersFilePath) : usersFilePath(usersFilePath) {
    readUsersFromFile();
}

void Bank::readUsersFromFile() {
    std::ifstream inFile(usersFilePath);
    if (!inFile) {
        // File doesn't exist or cannot be opened
        return;
    }

    std::string username, hashedPassword;
    double accountBalance;
    while (inFile >> username >> hashedPassword >> accountBalance) {
        users.emplace_back(username, hashedPassword, accountBalance);
    }

    inFile.close();
}

void Bank::writeUsersToFile() {
    std::ofstream outFile(usersFilePath);
    for (const auto& user : users) {
        outFile << user.getUsername() << " " << user.getHashedPassword() << " " << user.getAccountBalance() << std::endl;
    }
    outFile.close();
}

User* Bank::login(const std::string& username, const std::string& password) {
    for (auto& user : users) {
        if (user.getUsername() == username && user.verifyPassword(password)) {
            return &user;
        }
    }
    return nullptr;
}

bool Bank::registerUser(const std::string& username, const std::string& password) {
    // Check if the username already exists
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            return false; // Username already taken
        }
    }

    // Hash the password before storing it
    std::string hashedPassword = sha256(password);

    // Create a new user and add it to the users vector
    users.emplace_back(username, hashedPassword, 0.0);

    // Save the updated user data to file
    writeUsersToFile();

    return true;
}
