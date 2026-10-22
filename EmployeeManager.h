#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "Person.h"
#include "Client.h"
#include "Employee.h"
#include "Validation.h"
#include "BankStorage.h"
#include "fileManager.h"
#include "ClientManager.h"
#include "UI.h"

using namespace std;

class EmployeeManager {
public:
    static void printEmployeMenu() {
        cout << "(1) Display my info\n"
            << "(2) Update Password\n"
            << "(3) Add new client\n"
            << "(4) Search for client\n"
            << "(5) List all clients\n"
            << "(6) Edit client info\n"
            << "(7) Logout\n" << endl;
    }

    static void newClient(Employee* employee) {
        string name = UI::readLine("Client name: ");
        if (!Validation::validateName(name)) {
            cout << "Name should only include letters (3-20 chars)!" << endl;
            return;
        }
        string pass = UI::readLine("Client password (8-20 chars, no spaces): ");
        if (!Validation::validatePassword(pass)) {
            cout << "invalid Password " << endl;
            return;
        }
        double balance;
        if (!UI::readDouble("Starting balance (min 1500): ", balance)) return;
        if (!Validation::validateBalance(balance)) {
            cout << "invalid balance (minimum is 1500)" << endl;
            return;
        }
        Client c(0, name, pass, balance);   // real id is generated in FileManager::addClient
        employee->addClient(c);
        cout << "Client added successfully." << endl;
    }

    static void listAllClients(Employee* employee) {
        (void)employee;
        vector<Client*> list;
        for (auto& p : storage.Clients) list.push_back(&p.second);
        if (list.empty()) {
            cout << "No clients found." << endl;
            return;
        }
        sort(list.begin(), list.end(), [](Client* a, Client* b) { return a->getId() < b->getId(); });

        cout << left << setw(8) << "ID" << setw(24) << "Name" << "Balance" << endl;
        cout << string(46, '-') << endl;
        for (Client* c : list) {
            cout << left << setw(8) << c->getId() << setw(24) << c->getName() << c->getBalance() << endl;
        }
    }

    static void searchForClient(Employee* employee) {
        int id;
        if (!UI::readInt("Client ID: ", id)) return;
        Client* c = employee->searchClient(id);
        if (c == nullptr) {
            cout << "Client not found." << endl;
            return;
        }
        cout << endl;
        UI::showInfo(c, false);
    }

    static void editClientInfo(Employee* employee) {
        int id;
        if (!UI::readInt("Client ID: ", id)) return;
        Client* c = employee->searchClient(id);
        if (c == nullptr) {
            cout << "Client not found." << endl;
            return;
        }
        cout << "(press Enter to keep the current value)" << endl;

        string name = UI::readLine("New name: ");
        if (name.empty()) name = c->getName();
        else if (!Validation::validateName(name)) {
            cout << "Name should only include letters (3-20 chars)!" << endl;
            return;
        }

        string pass = UI::readLine("New password: ");
        if (pass.empty()) pass = c->getPass();
        else if (!Validation::validatePassword(pass)) {
            cout << "invalid Password " << endl;
            return;
        }

        double balance = c->getBalance();
        string bs = UI::readLine("New balance: ");
        if (!bs.empty()) {
            if (!UI::parseDouble(bs, balance)) { cout << "Invalid number." << endl; return; }
            if (!Validation::validateBalance(balance)) {
                cout << "invalid balance (minimum is 1500)" << endl;
                return;
            }
        }
        employee->editClient(id, name, pass, balance);
        cout << "Client updated successfully." << endl;
    }

    // storage.Employees must be loaded first (FileManager::getAllEmployees())
    static Employee* login(int id, string password) {
        auto it = storage.Employees.find(id);
        if (it == storage.Employees.end()) return nullptr;
        if (it->second.getPass() != password) return nullptr;
        return &it->second;
    }

    // true = stay logged in, false = logout
    static bool employeeOptions(Employee* employee) {
        UI::clearScreen();
        printEmployeMenu();
        int choice;
        if (!UI::readInt("Enter your choice: ", choice)) {
            UI::pause();
            return true;
        }
        switch (choice) {
        case 1: UI::showInfo(employee); break;
        case 2: ClientManager::updatePassword(employee); break;
        case 3: newClient(employee); break;
        case 4: searchForClient(employee); break;
        case 5: listAllClients(employee); break;
        case 6: editClientInfo(employee); break;
        case 7: return false;
        default: cout << "Invalid choice." << endl;
        }
        UI::pause();
        return true;
    }
};