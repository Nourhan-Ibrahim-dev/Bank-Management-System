#pragma once
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "Client.h"
#include "Employee.h"
#include "BankStorage.h"
#include "Parser.h"
using namespace std;

class FilesHelper {
public:
    static void saveLast(string fileName, int id) {
        ofstream file(fileName, ios::trunc);
        if (file.is_open()) {
            file << id;
            file.close();
        }
    }

    static int getLast(string fileName) {
        ifstream file(fileName);
        int id = 0;
        if (file.is_open()) {
            file >> id;
            file.close();
        }
        return id;
    }

    // always 2 decimals, never scientific notation (the default stream format keeps
    // only 6 significant digits, so 1236567.89 would be saved as 1.23657e+06)
    static string money(double value) {
        ostringstream out;
        out << fixed << setprecision(2) << value;
        return out.str();
    }

    static void saveClient(Client c) {
        ofstream file("Clients.txt", ios::app);
        if (file.is_open()) {
            file << c.getId() << '|' << c.getName() << '|' << c.getPass() << '|' << money(c.getBalance()) << endl;
            file.close();
            saveLast("ClientLastId.txt", c.getId());
        }
    }

    static void saveEmployee(string fileName, string lastIdFile, Employee e) {
        ofstream file(fileName, ios::app);
        if (file.is_open()) {
            file << e.getId() << '|' << e.getName() << '|' << e.getPass() << '|' << money(e.getSalary()) << endl;
            file.close();
            saveLast(lastIdFile, e.getId());
        }
    }

    static void clearFile(string fileName, string lastIdFile) {
        ofstream file1(fileName, ios::trunc);
        file1.close();

        ofstream file2(lastIdFile, ios::trunc);
        file2.close();
    }
};