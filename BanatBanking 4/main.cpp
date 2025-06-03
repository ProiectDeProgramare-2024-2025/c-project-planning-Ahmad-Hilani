#include "BankAccount.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "BankUtils.h"

const char* RESET   = "\033[0m";
const char* BOLD    = "\033[1m";
const char* RED     = "\033[31m";
const char* GREEN   = "\033[32m";
const char* YELLOW  = "\033[33m";
const char* BLUE    = "\033[34m";



void showMenu() {
    std::cout
        << BOLD << BLUE
        << "=== Welcome to the Banat Bank ===\n"
        << RESET
        << YELLOW
        << "1. " << RESET << "Register\n"
        << YELLOW
        << "2. " << RESET << "Login\n"
        << YELLOW
        << "3. " << RESET << "Exit\n"
        << RESET;
}

void userMenu(BankAccount& acc) {
    int choice;
    do {
        std::cout
            << "\n" << BOLD << GREEN
            << "--- Welcome, " << acc.getAccountHolder() << " ---\n"
            << RESET
            << YELLOW
            << "1. " << RESET << "View Balance\n"
            << YELLOW
            << "2. " << RESET << "Deposit\n"
            << YELLOW
            << "3. " << RESET << "Withdraw\n"
            << YELLOW
            << "4. " << RESET << "Delete Account\n"
            << YELLOW
            << "5. " << RESET << "Send Money\n"
            << YELLOW
            << "6. " << RESET << "Logout\n"
            << BOLD << BLUE
            << "Enter your choice: "
            << RESET;

        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                system("cls");
                std::cout << GREEN
                          << "Current balance: $"
                          << acc.getBalance()
                          << RESET << "\n";
                break;

            case 2: {
                double amt;
                std::cout << BLUE
                          << "Enter amount to deposit: "
                          << RESET;
                std::cin >> amt;
                system("cls");
                acc.deposit(amt);
                break;
            }

            case 3: {
                double amt;
                std::cout << BLUE
                          << "Enter amount to withdraw: "
                          << RESET;
                std::cin >> amt;
                system("cls");
                acc.withdraw(amt);
                break;
            }

            case 4:
                std::cout << RED;
                acc.deleteAccount();
                std::cout << RESET
                          << "Account deleted. Logging out.\n";
                return;

            case 5: {
                while (true) {
                    std::string target;
                    double amt;
                    std::cout << BLUE
                              << "Enter recipient account ID (or 'q' to cancel): "
                              << RESET;
                    std::getline(std::cin, target);
                    if (target == "q") {
                        system("cls");
                        std::cout << YELLOW
                                  << "Transfer cancelled.\n"
                                  << RESET;
                        break;
                    }

                    std::cout << BLUE
                              << "Enter amount to send: "
                              << RESET;
                    std::cin >> amt;
                    std::cin.ignore();

                    if (acc.sendMoney(target, amt)) {
                        break;
                    }
                }
                break;
            }

            case 6:
                std::cout << BLUE
                          << "Logging out...\n"
                          << RESET;
                break;

            default:
                system("cls");
                std::cout << RED
                          << "Invalid option. Try again.\n"
                          << RESET;
        }

    } while (choice != 6);
}

void registerUser() {
    std::string name, pass;
    std::cout << "Enter name: ";
    std::getline(std::cin, name);
    std::cout << "Enter password: ";
    std::getline(std::cin, pass);

    BankAccount newAcc(name, pass);
    newAcc.saveToFile();

    system("cls");
    std::cout << "Account created successfully. Here is your ID: "<<newAcc.getID()
    <<std::endl;
}

bool loginUser(BankAccount& loggedInAcc) {
    std::string ID, pass;
    std::cout << "Enter ID: ";
    std::getline(std::cin, ID);
    if (ID.size() != 20 || ID.substr(0,2) != "AL") {
        system("cls");
        std::cerr << "Account ID must be 20 characters, starting with \"AL\".\n";
        return false;
    }
    std::cout << "Enter password: ";
    std::getline(std::cin, pass);

    std::ifstream file("accounts.txt");
    std::string line;
    std::string hashedInput = BankUtils::hashPassword(pass);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string id, fileName, fileHash, balanceStr;

        std::getline(ss, id, ',');
        std::getline(ss, fileName, ',');
        std::getline(ss, fileHash, ',');
        std::getline(ss, balanceStr, ',');



        double balance;
        balance = std::stod(balanceStr);

        if (id == ID && fileHash == hashedInput) {

            loggedInAcc = BankAccount(id, fileName, fileHash, balance);

            system("cls");
            std::cout << "Login successful!\n";
            return true;
        }

    }
    system("cls");
    std::cout << "Login failed. Invalid credentials.\n";
    return false;
}


int main() {
    BankAccount currentUser;
    system("cls");


    while (true) {
        showMenu();
        int choice;
        std::cout<<"Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();


        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            if (loginUser(currentUser)) {
                userMenu(currentUser);
                break;
            }
        } else if (choice == 3) {
            std::cout << BOLD << GREEN
            << "Bye!\n"<< RESET;
            break;
        } else {
            system("cls");
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.