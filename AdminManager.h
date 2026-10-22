#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "Person.h"
#include "Employee.h"
#include "Admin.h"
#include "Validation.h"
#include "BankStorage.h"
#include "fileManager.h"
#include "ClientManager.h"
#include "EmployeeManager.h"
#include "UI.h"

using namespace std;

class AdminManager {
public:
    static void printEmployeeMenu() {
        cout << "(1) Display my info\n"
            << "(2) Update Password\n"
            << "(3) Add new client\n"
            << "(4) Search for client\n"
            << "(5) List all clients\n"
            << "(6) Edit client info\n"
            << "(7) Add new Employee\n"
            << "(8) Search for Employee\n"
            << "(9) List all Employees\n"
            << "(10) Edit Employee info\n"
            << "(11) Logout\n" << endl;
    }

    // storage.Admins must be loaded first (FileManager::getAllAdmins())
    static Admin* login(int id, string password) {
        auto it = storage.Admins.find(id);
        if (it == storage.Admins.end()) return nullptr;
        if (it->second.getPass() != password) return nullptr;
        return &it->second;
    }

    static void newEmployee(Admin* admin) {
        string name = UI::readLine("Employee name: ");
        if (!Validation::validateName(name)) {
            cout << "Name should only include letters (3-20 chars)!" << endl;
            return;
        }
        string pass = UI::readLine("Employee password (8-20 chars, no spaces): ");
        if (!Validation::validatePassword(pass)) {
            cout << "invalid Password " << endl;
            return;
        }
        double salary;
        if (!UI::readDouble("Salary (min 5000): ", salary)) return;
        if (!Validation::validateSalary(salary)) {
            cout << "invalid salary (minimum is 5000)" << endl;
            return;
        }
        Employee e(0, name, pass, salary);   // real id is generated in FileManager::addEmployee
        admin->addEmployee(e);
        cout << "Employee added successfully." << endl;
    }

    static void searchForEmployee(Admin* admin) {
        int id;
        if (!UI::readInt("Employee ID: ", id)) return;
        Employee* e = admin->searchEmployee(id);
        if (e == nullptr) {
            cout << "Employee not found." << endl;
            return;
        }
        cout << endl;
        UI::showInfo(e, false);
    }

    static void listAllEmployees(Admin* admin) {
        (void)admin;
        vector<Employee*> list;
        for (auto& p : storage.Employees) list.push_back(&p.second);
        if (list.empty()) {
            cout << "No employees found." << endl;
            return;
        }
        sort(list.begin(), list.end(), [](Employee* a, Employee* b) { return a->getId() < b->getId(); });

        cout << left << setw(8) << "ID" << setw(24) << "Name" << "Salary" << endl;
        cout << string(46, '-') << endl;
        for (Employee* e : list) {
            cout << left << setw(8) << e->getId() << setw(24) << e->getName() << e->getSalary() << endl;
        }
    }

    static void editEmployeeInfo(Admin* admin) {
        int id;
        if (!UI::readInt("Employee ID: ", id)) return;
        Employee* e = admin->searchEmployee(id);
        if (e == nullptr) {
            cout << "Employee not found." << endl;
            return;
        }
        cout << "(press Enter to keep the current value)" << endl;

        string name = UI::readLine("New name: ");
        if (name.empty()) name = e->getName();
        else if (!Validation::validateName(name)) {
            cout << "Name should only include letters (3-20 chars)!" << endl;
            return;
        }

        string pass = UI::readLine("New password: ");
        if (pass.empty()) pass = e->getPass();
        else if (!Validation::validatePassword(pass)) {
            cout << "invalid Password " << endl;
            return;
        }

        double salary = e->getSalary();
        string ss = UI::readLine("New salary: ");
        if (!ss.empty()) {
            if (!UI::parseDouble(ss, salary)) { cout << "Invalid number." << endl; return; }
            if (!Validation::validateSalary(salary)) {
                cout << "invalid salary (minimum is 5000)" << endl;
                return;
            }
        }
        admin->editEmployee(id, name, pass, salary);
        cout << "Employee updated successfully." << endl;
    }

    // true = stay logged in, false = logout
    static bool AdminOptions(Admin* admin) {
        UI::clearScreen();
        printEmployeeMenu();
        int choice;
        if (!UI::readInt("Enter your choice: ", choice)) {
            UI::pause();
            return true;
        }
        switch (choice) {
        case 1:  UI::showInfo(admin); break;
        case 2:  ClientManager::updatePassword(admin); break;
        case 3:  EmployeeManager::newClient(admin); break;
        case 4:  EmployeeManager::searchForClient(admin); break;
        case 5:  EmployeeManager::listAllClients(admin); break;
        case 6:  EmployeeManager::editClientInfo(admin); break;
        case 7:  newEmployee(admin); break;
        case 8:  searchForEmployee(admin); break;
        case 9:  listAllEmployees(admin); break;
        case 10: editEmployeeInfo(admin); break;
        case 11: return false;
        default: cout << "Invalid choice." << endl;
        }
        UI::pause();
        return true;
    }
};