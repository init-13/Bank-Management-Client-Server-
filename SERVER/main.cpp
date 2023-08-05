#include "bank.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h> // For Windows
#else
#include <cstdlib> // For Linux and macOS
#include <unistd.h> // For sleep() on Unix-based systems
#endif

void clearTerminalWithDelay() {
#ifdef _WIN32
    // For Windows
    Sleep(2000); // 2 seconds delay
    system("cls");
#else
    // For Linux and macOS
    sleep(2); // 2 seconds delay
    system("clear");
#endif
}

// Function to display the main menu options
void displayMainMenu() {
    std::cout << "-------------------------" << std::endl;
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Register" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Please choose an option: ";
}

int main() {
    const std::string usersFilePath = "database/users.txt";
    Bank bank(usersFilePath);

    int choice;
    do {
        // #ifdef _WIN32 
        //         Sleep(2000);
        // #else 
        //         sleep(2);
        // #endif
        
        // clearTerminalWithDelay();
        
        std::cout<<std::endl
        <<std::endl;
        displayMainMenu();
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string username, password;
            std::cout << "Enter your username: ";
            std::cin >> username;
            std::cout << "Enter your password: ";
            std::cin >> password;

            User* currentUser = bank.login(username, password);
            if (currentUser != nullptr) {
                std::cout << "Login successful!" << std::endl;

                // Display account options menu
                bool loggedIn = true;
                while (loggedIn) {
                    std::cout << "-------------------------" << std::endl;
                    std::cout << "1. View Account Details" << std::endl;
                    std::cout << "2. Withdraw" << std::endl;
                    std::cout << "3. Deposit" << std::endl;
                    std::cout << "4. Change Password" << std::endl;
                    std::cout << "0. Logout" << std::endl;
                    std::cout << "-------------------------" << std::endl;
                    std::cout << "Please choose an option: ";
                    std::cin >> choice;

                    switch (choice) {
                    case 1:
                        currentUser->viewAccountDetails();
                        

                        break;
                    case 2: {
                        double amount;
                        std::cout << "Enter amount to withdraw: ";
                        std::cin >> amount;
                        currentUser->withdraw(amount);
                        break;
                    }
                    case 3: {
                        double amount;
                        std::cout << "Enter amount to deposit: ";
                        std::cin >> amount;
                        currentUser->deposit(amount);
                        break;
                    }
                    case 4: {
                        std::string newPassword;
                        std::cout << "Enter new password: ";
                        std::cin >> newPassword;
                        currentUser->changePassword(newPassword);
                        break;
                    }
                    case 0:
                        loggedIn = false;
                        std::cout << "Logged out." << std::endl;
                        break;
                    default:
                        std::cout << "Invalid option. Please try again." << std::endl;
                    }
                }
            } else {
                std::cout << "Login failed. Invalid username or password." << std::endl;
            }
            break;
        }
        case 2: {
            std::string username, password;
            std::cout << "Enter your desired username: ";
            std::cin >> username;
            std::cout << "Enter your desired password: ";
            std::cin >> password;

            if (bank.registerUser(username, password)) {
                std::cout << "Registration successful. You can now login." << std::endl;
            } else {
                std::cout << "Registration failed. Username already taken." << std::endl;
            }
            break;
        }
        case 0:
            std::cout << "Exiting program. Goodbye!" << std::endl;
            break;
        default:
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (choice != 0);

    return 0;
}
