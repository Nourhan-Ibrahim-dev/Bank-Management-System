
#pragma once
#include "Client.h"
#include "Employee.h"
#include "Admin.h"
#include "DataSourceInterface.h"
#include "FilesHelper.h"
#include "BankStorage.h"
#include "Parser.h"
#include <fstream>
using namespace std;

class FileManager : public DataSourceInterface {
public:
    // ---------- add ----------
    void addClient(Client obj) override {
        int id = FilesHelper::getLast("ClientLastId.txt") + 1;
        obj.setId(id);
        FilesHelper::saveClient(obj);
        storage.Clients.emplace(id, obj);
    }

    void addEmployee(Employee obj) override {
        int id = FilesHelper::getLast("EmployeeLastId.txt") + 1;
        obj.setId(id);
        FilesHelper::saveEmployee("Employees.txt", "EmployeeLastId.txt", obj);
        storage.Employees.emplace(id, obj);
    }

    void addAdmin(Admin obj) override {
        int id = FilesHelper::getLast("AdminLastId.txt") + 1;
        obj.setId(id);
        FilesHelper::saveEmployee("Admins.txt", "AdminLastId.txt", obj);
        storage.Admins.emplace(id, obj);
    }

    // ---------- get all ----------
    void getAllClients() override {
        storage.Clients.clear();
        ifstream file("Clients.txt");
        string line;
        while (getline(file, line)) {
            if (line == "") continue;
            if (Parser::split(line).size() < 4) continue;   // skip broken lines
            try {
                Client c = Parser::parseToClient(line);
                storage.Clients.emplace(c.getId(), c);
            }
            catch (...) { continue; }
        }
    }

    void getAllEmployees() override {
        storage.Employees.clear();
        ifstream file("Employees.txt");
        string line;
        while (getline(file, line)) {
            if (line == "") continue;
            if (Parser::split(line).size() < 4) continue;   // skip broken lines
            try {
                Employee e = Parser::parseToEmployee(line);
                storage.Employees.emplace(e.getId(), e);
            }
            catch (...) { continue; }
        }
    }

    void getAllAdmins() override {
        storage.Admins.clear();
        ifstream file("Admins.txt");
        string line;
        while (getline(file, line)) {
            if (line == "") continue;
            if (Parser::split(line).size() < 4) continue;   // skip broken lines
            try {
                Admin a = Parser::parseToAdmin(line);
                storage.Admins.emplace(a.getId(), a);
            }
            catch (...) { continue; }
        }
    }

    // ---------- remove all ----------
    void removeAllClients() override {
        FilesHelper::clearFile("Clients.txt", "ClientLastId.txt");
        storage.Clients.clear();
    }

    void removeAllEmployees() override {
        FilesHelper::clearFile("Employees.txt", "EmployeeLastId.txt");
        storage.Employees.clear();
    }

    void removeAllAdmins() override {
        FilesHelper::clearFile("Admins.txt", "AdminLastId.txt");
        storage.Admins.clear();
    }

    // ---------- update ----------
    void updateClient() {
        ofstream file("Clients.txt");
        for (auto& p : storage.Clients) {
            file << p.second.getId() << '|' << p.second.getName() << '|'
                << p.second.getPass() << '|' << FilesHelper::money(p.second.getBalance()) << endl;
        }
    }

    void updateEmployee() {
        ofstream file("Employees.txt");
        for (auto& p : storage.Employees) {
            file << p.second.getId() << '|' << p.second.getName() << '|'
                << p.second.getPass() << '|' << FilesHelper::money(p.second.getSalary()) << endl;
        }
    }

    void updateAdmin() {
        ofstream file("Admins.txt");
        for (auto& p : storage.Admins) {
            file << p.second.getId() << '|' << p.second.getName() << '|'
                << p.second.getPass() << '|' << FilesHelper::money(p.second.getSalary()) << endl;
        }
    }
};

// ======================================================================
// Employee / Admin functions that need FileManager + storage.
// They are defined here (after FileManager is complete) to avoid a
// circular include between Employee.h/Admin.h and Filemanager.h.
// ======================================================================

// ---------- Employee ----------
inline void Employee::addClient(Client& client) {
    FileManager fm;
    fm.addClient(client);
}

inline Client* Employee::searchClient(int id) {
    auto it = storage.Clients.find(id);
    if (it == storage.Clients.end()) {
        return nullptr;
    }
    return &it->second;
}

inline void Employee::listClient() {
    for (auto& p : storage.Clients) {
        p.second.displayDetails();
        cout << "-------" << endl;
    }
}

inline void Employee::editClient(int id, string name, string password, double balance) {
    Client* c = searchClient(id);
    if (c == nullptr) {
        cout << "Client not found" << endl;
        return;
    }
    c->setName(name);
    c->setPassword(password);
    c->setBalance(balance);

    FileManager fm;
    fm.updateClient();
}

// ---------- Admin ----------
inline void Admin::addEmployee(Employee& employee) {
    FileManager fm;
    fm.addEmployee(employee);
}

inline Employee* Admin::searchEmployee(int id) {
    auto it = storage.Employees.find(id);
    if (it == storage.Employees.end()) {
        return nullptr;
    }
    return &it->second;
}

inline void Admin::editEmployee(int id, string name, string password, double salary) {
    Employee* e = searchEmployee(id);
    if (e == nullptr) {
        cout << "Employee not found" << endl;
        return;
    }
    e->setName(name);
    e->setPassword(password);
    e->setSalary(salary);

    FileManager fm;
    fm.updateEmployee();
}

inline void Admin::listEmployee() {
    for (auto& p : storage.Employees) {
        p.second.displayDetails();
        cout << "-------" << endl;
    }
}