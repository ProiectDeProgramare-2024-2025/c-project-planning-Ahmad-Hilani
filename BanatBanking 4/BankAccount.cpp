//
// Created by lozog on 5/23/2025.
//

// BankAccount.cpp

#include "BankAccount.h"
#include "BankUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


BankAccount::BankAccount() {
    this->accountID = "";
    this->accountHolder = "";
    this->password = "";
    this->balance = 0.0;
}

BankAccount::BankAccount(const std::string& name, const std::string& pass) {
    this->accountID = BankUtils::generateAccountID();
    this->accountHolder = name;
    this->password = BankUtils::hashPassword(pass);
    this->balance = 0.0;
}

BankAccount::BankAccount(const std::string& id, const std::string& name, const std::string& hash, double bal) {
    this->accountID = id;
    this->accountHolder = name;
    this->password = hash;
    this->balance = bal;
}

std::string BankAccount::getID() const {
    return accountID;
}

std::string BankAccount::getAccountHolder() const {
    return accountHolder;
}

double BankAccount::getBalance() const {
    return balance;
}

void BankAccount::saveToFile(bool overwrite) const {
    std::ifstream inFile("accounts.txt");
    std::vector<std::string> lines;
    bool found = false;

    if (overwrite && inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream ss(line);
            std::string id;
            std::getline(ss, id, ',');

            if (id == accountID) {
                std::ostringstream updated;
                updated << accountID << "," << accountHolder << "," << password << "," << balance;
                lines.push_back(updated.str());
                found = true;
            } else {
                lines.push_back(line);
            }
        }
        inFile.close();
    }

    std::ofstream outFile("accounts.txt", overwrite ? std::ios::trunc : std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Error writing to accounts.txt\n";
        return;
    }

    if (overwrite) {
        for (const auto& l : lines) {
            outFile << l << "\n";
        }
        if (!found) {
            outFile << accountID << "," << accountHolder << "," << password << "," << balance << "\n";
        }
    } else {
        outFile << accountID << "," << accountHolder << "," << password << "," << balance << "\n";
    }

    outFile.close();
}

bool BankAccount::loadAccount(const std::string& id, const std::string& plainPassword, BankAccount& outAccount) {
    std::ifstream inFile("accounts.txt");
    std::string line;
    std::string hashedInput = BankUtils::hashPassword(plainPassword);

    while (std::getline(inFile, line)) {
        std::istringstream ss(line);
        std::string fileID, name, hash, balStr;
        std::getline(ss, fileID, ',');
        std::getline(ss, name, ',');
        std::getline(ss, hash, ',');
        std::getline(ss, balStr, ',');

        if (fileID == id && hash == hashedInput) {
            double bal = std::stod(balStr);
            outAccount = BankAccount(fileID, name, hash, bal);
            return true;
        }
    }

    return false;
}

bool BankAccount::deposit(double amount) {
    if (amount <= 0) return false;
    balance += amount;
    saveToFile(true);
    return true;
}

bool BankAccount::withdraw(double amount) {
    if (amount <= 0 || amount > balance) return false;
    balance -= amount;
    saveToFile(true);
    return true;
}

bool BankAccount::sendMoney(const std::string& targetID, double amount) {
    if (amount <= 0 || amount > balance) {
        std::cout << "Insufficient funds or invalid amount.\n";
        return false;
    }

    BankAccount targetAcc;
    if (!loadAccount(targetID, "", targetAcc)) {
        std::cout << "Target account not found.\n";
        return false;
    }

    targetAcc.balance += amount;
    balance -= amount;

    saveToFile(true);
    targetAcc.saveToFile(true);

    std::cout << "Successfully sent $" << amount << " to " << targetAcc.getAccountHolder() << " (" << targetID << ")\n";
    return true;
}

void BankAccount::deleteAccount() const {
    std::ifstream inFile("accounts.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(inFile, line)) {
        std::istringstream ss(line);
        std::string id;
        std::getline(ss, id, ',');

        if (id != accountID) {
            lines.push_back(line);
        }
    }
    inFile.close();

    std::ofstream outFile("accounts.txt", std::ios::trunc);
    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

    std::cout << "Account successfully deleted.\n";
}
