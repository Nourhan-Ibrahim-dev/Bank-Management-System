#pragma once
#include "Person.h"
#include "Client.h"

class Employee : public Person {
protected:
    double salary;

public:
    Employee(int id, string name, string password, double salary)
        : Person(id, name, password), salary(5000) {
        setSalary(salary);
    }

    void setSalary(double salary) {
        if (salary < 5000) {
            cout << "Not Valid Salary must be at least 5000!" << endl;
            return;
        }
        this->salary = salary;
    }

    double getSalary() { return salary; }

    void displayDetails() override {
        Person::displayDetails();
        cout << "Salary : " << salary << endl;
    }

    // These use storage + FileManager, so they are only declared here.
    // Their bodies are at the bottom of Filemanager.h
    void addClient(Client& client);
    Client* searchClient(int id);
    void listClient();
    void editClient(int id, string name, string password, double balance);
};