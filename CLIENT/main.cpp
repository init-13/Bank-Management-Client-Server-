#include <iostream>
#include "BankClient.h"

void printMainMenu() {
    std::cout << "==== Banking System ====" << std::endl;
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Register" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "========================" << std::endl;
}

void printLoggedInMenu() {
    std::cout << "==== Welcome, User! ====" << std::endl;
    std::cout << "1. View Balance" << std::endl;
    std::cout << "2. Withdraw" << std::endl;
    std::cout << "3. Deposit" << std::endl;
    std::cout << "4. Change Password" << std::endl;
    std::cout << "5. Logout" << std::endl;
    std::cout << "========================" << std::endl;
}

int main() {
    const int serverPort = 8080; // Replace with the actual server port

    BankClient client(serverPort);

    if (!client.connectToServer()) {
        std::cerr << "Failed to connect to the server." << std::endl;
        return 1;
    }

    bool isLoggedIn = false;
    int choice;

    while (true) {
        if (!isLoggedIn) {
            printMainMenu();
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (choice == 1) {
                std::string username, password;
                std::cout << "Enter username: ";
                std::cin >> username;
                std::cout << "Enter password: ";
                std::cin >> password;

                // Send the login request to the server
                std::string request = "LOGIN|" + username + "|" + password;
                std::string response = client.sendRequest(request);

                // Process the response
                if (response.find("SUCCESS|") == 0) {
                    isLoggedIn = true;
                    std::cout << "Login successful!" << std::endl;
                } else {
                    std::cout << "Login failed. " << response.substr(8) << std::endl;
                }
            } else if (choice == 2) {
                std::string username, password;
                std::cout << "Enter username: ";
                std::cin >> username;
                std::cout << "Enter password: ";
                std::cin >> password;

                // Send the register request to the server
                std::string request = "REGISTER|" + username + "|" + password;
                std::string response = client.sendRequest(request);

                // Process the response
                if (response.find("SUCCESS|") == 0) {
                    std::cout << "Registration successful!" << std::endl;
                } else {
                    std::cout << "Registration failed. " << response.substr(8) << std::endl;
                }
            } else if (choice == 3) {
                std::cout << "Exiting..." << std::endl;
                break;
            } else {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
        } else {
            printLoggedInMenu();
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (choice == 1) {
                // Send the view balance request to the server
                std::string request = "VIEW_BALANCE|username"; // Replace 'username' with the actual logged-in username
                std::string response = client.sendRequest(request);

                // Process the response
                if (response.find("SUCCESS|") == 0) {
                    double balance = std::stod(response.substr(8));
                    std::cout << "Your account balance: " << balance << std::endl;
                } else {
                    std::cout << "Failed to get account balance. " << response.substr(8) << std::endl;
                }
            } else if (choice == 2) {
                // Withdraw implementation here
            } else if (choice == 3) {
                // Deposit implementation here
            } else if (choice == 4) {
                // Change password implementation here
            } else if (choice == 5) {
                isLoggedIn = false;
                std::cout << "Logged out successfully!" << std::endl;
            } else {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
        }
    }

    client.disconnectFromServer();
    return 0;
}
