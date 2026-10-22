#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include "Client.h"
#include "Employee.h"
#include "Admin.h"
using namespace std;

class Parser
{
public:
    // Example line: 1|Mohamed|mohamed123|5000
    static vector<string> split(string line) {
        vector<string> parts;

        stringstream ss(line);
        string part;
        while (getline(ss, part, '|')) {
            parts.push_back(part);
        }
        return parts;
    }

    // [0]    [1]       [2]          [3]
    // id     name      password     balance
    static Client parseToClient(string line) {
        vector<string> parts = split(line);
        return Client(stoi(parts[0]), parts[1], parts[2], stod(parts[3]));
    }

    // [0]    [1]       [2]          [3]
    // id     name      password     salary
    static Employee parseToEmployee(string line) {
        vector<string> parts = split(line);
        return Employee(stoi(parts[0]), parts[1], parts[2], stod(parts[3]));
    }

    // [0]    [1]       [2]          [3]
    // id     name      password     salary
    static Admin parseToAdmin(string line) {
        vector<string> parts = split(line);
        return Admin(stoi(parts[0]), parts[1], parts[2], stod(parts[3]));
    }
};