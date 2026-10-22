#pragma once
#include <unordered_map>
#include "Employee.h"
#include "Client.h"
#include "Admin.h"
using namespace std;

class BankStorage
{
public:
    unordered_map<int, Employee> Employees;
    unordered_map<int, Client> Clients;
    unordered_map<int, Admin> Admins;
};

inline BankStorage storage;