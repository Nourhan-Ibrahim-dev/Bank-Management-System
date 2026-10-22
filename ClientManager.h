#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Person.h"
#include "Client.h"
#include "Employee.h"
#include "Admin.h"
#include "Validation.h"
#include "BankStorage.h"
#include "fileManager.h"
#include "UI.h"

using namespace std;

class ClientManager {
public:
    static void printClientMenu() {
        cout << "(1) Display my info\n"
            << "(2) Update Password\n"
            << "(3) Check balance\n"
            << "(4) Deposit\n"
            << "(5) Withdraw\n"
            << "(6) Transfer to another client\n"
            << "(7) Logout\n" << endl;
    }

    // works for Client, Employee and Admin: saves into the right file
    static void updatePassword(Person* person) {
        string current = UI::readLine("Current password: ");
        if (current != person->getPass()) {
            cout << "Wrong current password." << endl;
            return;
        }
        string newPass = UI::readLine("New password (8-20 chars, no spaces): ");
        if (!Validation::validatePassword(newPass)) {
            cout << "invalid Password " << endl;
            return;
        }
        person->setPassword(newPass);

        FileManager fm;
        if (dynamic_cast<Admin*>(person))         fm.updateAdmin();
        else if (dynamic_cast<Employee*>(person)) fm.updateEmployee();
        else                                      fm.updateClient();
        cout << "Password updated successfully." << endl;
    }

    // storage.Clients must be loaded first (FileManager::getAllClients())
    static Client* login(int id, string password) {
        auto it = storage.Clients.find(id);
        if (it == storage.Clients.end()) return nullptr;
        if (it->second.getPass() != password) return nullptr;
        return &it->second;
    }

    // true = stay logged in, false = logout
    static bool clientOptions(Client* client) {
        UI::clearScreen();
        printClientMenu();
        int choice;
        if (!UI::readInt("Enter your choice: ", choice)) {
            UI::pause();
            return true;
        }

        FileManager fm;
        switch (choice) {
        case 1:
            UI::showInfo(client);
            break;
        case 2:
            updatePassword(client);
            break;
        case 3:
            client->checkBalance();
            break;
        case 4: {
            double amount;
            if (!UI::readDouble("Amount to deposit: ", amount)) break;
            if (amount <= 0) { cout << "Invalid amount" << endl; break; }
            client->deposit(amount);
            fm.updateClient();
            cout << "Deposit done. ";
            client->checkBalance();
            break;
        }
        case 5: {
            double amount;
            if (!UI::readDouble("Amount to withdraw: ", amount)) break;
            if (amount <= 0) { cout << "Invalid amount" << endl; break; }
            double before = client->getBalance();
            client->withdraw(amount);
            if (client->getBalance() != before) fm.updateClient();
            break;
        }
        case 6: {
            int recipientId;
            double amount;
            if (!UI::readInt("Recipient ID: ", recipientId)) break;
            auto it = storage.Clients.find(recipientId);
            if (it == storage.Clients.end()) {
                cout << "Recipient not found." << endl;
                break;
            }
            if (&it->second == client) {
                cout << "You can't transfer to yourself." << endl;
                break;
            }
            if (!UI::readDouble("Amount to transfer: ", amount)) break;
            double before = client->getBalance();
            client->transferTo(amount, it->second);
            if (client->getBalance() != before) fm.updateClient();
            break;
        }
        case 7:
            return false;
        default:
            cout << "Invalid choice." << endl;
        }
        UI::pause();
        return true;
    }
};