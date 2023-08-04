#include <iostream>
#include "SHA.h"
class User {
private:
    std::string username;
    std::string hashedPassword;
    double accountBalance;

public:
    User(const std::string& username, const std::string& hashedPassword, double accountBalance);
    
    // Getters
    const std::string& getUsername() const;
    double getAccountBalance() const;
    const std::string& getHashedPassword() const;

    // Methods
    bool verifyPassword(const std::string& password) const;
    void changePassword(const std::string& newPassword);
    void viewAccountDetails() const;
    void withdraw(double amount);
    void deposit(double amount);
};


User::User(const std::string& username, const std::string& hashedPassword, double accountBalance)
    : username(username), hashedPassword(hashedPassword), accountBalance(accountBalance) {}

const std::string& User::getUsername() const {
    return username;
}

double User::getAccountBalance() const {
    return accountBalance;
}

const std::string& User::getHashedPassword() const {
        return hashedPassword;
    }

bool User::verifyPassword(const std::string& password) const {
    return hashedPassword == sha256(password);
}

void User::changePassword(const std::string& newPassword) {
    hashedPassword = sha256(newPassword);
}

void User::viewAccountDetails() const {
    std::cout << "Username: " << username << std::endl;
    std::cout << "Account Balance: " << accountBalance << std::endl;
}

void User::withdraw(double amount) {
    if (amount > 0 && amount <= accountBalance) {
        accountBalance -= amount;
        std::cout << "Withdrawal successful. Remaining balance: " << accountBalance << std::endl;
    } else {
        std::cout << "Withdrawal failed. Insufficient balance." << std::endl;
    }
}

void User::deposit(double amount) {
    if (amount > 0) {
        accountBalance += amount;
        std::cout << "Deposit successful. New balance: " << accountBalance << std::endl;
    } else {
        std::cout << "Invalid deposit amount." << std::endl;
    }
}
