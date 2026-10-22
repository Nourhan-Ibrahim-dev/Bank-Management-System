#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include "Client.h"
#include "Employee.h"

using namespace std;

// Small helpers shared by all screens: safe input, clear screen, info card.
class UI {
public:
    static void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    // ---------- colors (ANSI) ----------
    static constexpr const char* CLR_GOLD = "\033[38;5;220m";
    static constexpr const char* CLR_WHITE = "\033[1;97m";
    static constexpr const char* CLR_GRAY = "\033[90m";
    static constexpr const char* CLR_RESET = "\033[0m";

    // black background + white text, and turns on ANSI colors on Windows.
    // To change the background use another code: color 0F = black, 1F = dark blue, 5F = purple ...
    static void setTheme() {
#ifdef _WIN32
        system("color 0F");
     
#endif
    }

    static string trim(const string& s) {
        size_t b = s.find_first_not_of(" \t\r\n");
        if (b == string::npos) return "";
        size_t e = s.find_last_not_of(" \t\r\n");
        return s.substr(b, e - b + 1);
    }

    // always reads a full line, so cin never gets stuck
    static string readLine(const string& prompt) {
        cout << prompt;
        string s;
        if (!getline(cin, s)) {          // input closed (Ctrl+Z / EOF)
            cout << endl;
            exit(0);
        }
        return trim(s);
    }

    static bool parseInt(const string& s, int& v) {
        try {
            size_t pos = 0;
            v = stoi(s, &pos);
            return pos == s.size();
        }
        catch (...) { return false; }
    }

    static bool parseDouble(const string& s, double& v) {
        try {
            size_t pos = 0;
            v = stod(s, &pos);
            // stod accepts "nan" and "inf": reject them, they would corrupt balances
            return pos == s.size() && isfinite(v);
        }
        catch (...) { return false; }
    }

    static bool readInt(const string& prompt, int& v) {
        if (!parseInt(readLine(prompt), v)) {
            cout << "Invalid number." << endl;
            return false;
        }
        return true;
    }

    static bool readDouble(const string& prompt, double& v) {
        if (!parseDouble(readLine(prompt), v)) {
            cout << "Invalid number." << endl;
            return false;
        }
        return true;
    }

    static void pause() {
        readLine("\nPress Enter to continue...");
    }

    static void showInfo(Client* c, bool showPass = true) {
        cout << "id: " << c->getId() << endl;
        cout << "name: " << c->getName() << endl;
        if (showPass) cout << "password: " << c->getPass() << endl;
        cout << "Balance: " << c->getBalance() << endl;
        cout << "\n===============\n" << endl;
    }

    // works for Employee and Admin
    static void showInfo(Employee* e, bool showPass = true) {
        cout << "id: " << e->getId() << endl;
        cout << "name: " << e->getName() << endl;
        if (showPass) cout << "password: " << e->getPass() << endl;
        cout << "Salary: " << e->getSalary() << endl;
        cout << "\n===============\n" << endl;
    }
};