#pragma once
#include "Employee.h"
#include "Client.h"

// Admin inherits addClient / searchClient / listClient / editClient from Employee.
class Admin : public Employee
{
public:
    Admin(int id, string name, string password, double salary)
        : Employee(id, name, password, salary)
    {
    }

    // Declared only: bodies are at the bottom of Filemanager.h
    void addEmployee(Employee& employee);
    Employee* searchEmployee(int id);
    void editEmployee(int id, string name, string password, double salary);
    void listEmployee();

    void displayDetails() override
    {
        Employee::displayDetails();
    }
};