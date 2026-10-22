#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include "fileManager.h"
#include "ClientManager.h"
#include "EmployeeManager.h"
#include "AdminManager.h"
#include "UI.h"

using namespace std;

class Screens {
private:
    inline static bool exitApp = false;

    // after login: (1) Options / (0) Exit. Returns true for Options.
    static bool optionsOrExit() {
        cout << "(1) Options\t\t(0) Exit" << endl;
        int c;
        while (true) {
            if (!UI::readInt("..::Enter Choice: ", c)) continue;
            if (c == 0 || c == 1) return c == 1;
            invalid(c);
        }
    }

    // runs only when there is no admin at all (first launch)
    static void firstRunSetup(FileManager& fm) {
        UI::clearScreen();
        title();
        cout << "No admin account found. Let's create the first admin.\n" << endl;

        string name, pass;
        double salary;
        while (true) {
            name = UI::readLine("Name: ");
            if (Validation::validateName(name)) break;
            cout << "Name should only include letters (3-20 chars)!" << endl;
        }
        while (true) {
            pass = UI::readLine("Password (8-20 chars, no spaces): ");
            if (Validation::validatePassword(pass)) break;
            cout << "invalid Password " << endl;
        }
        while (true) {
            if (UI::readDouble("Salary (min 5000): ", salary) && Validation::validateSalary(salary)) break;
            cout << "invalid salary (minimum is 5000)" << endl;
        }

        Admin admin(0, name, pass, salary);   // real id is generated in FileManager::addAdmin
        fm.addAdmin(admin);

        cout << "\nAdmin created successfully." << endl;
        cout << "Your login ID is: " << storage.Admins.begin()->second.getId() << endl;
        UI::pause();
    }

    // ---------- banner helpers ----------
    static void center(const string& text, int width = 80) {
        int pad = (width - (int)text.size()) / 2;
        if (pad < 0) pad = 0;
        cout << string(pad, ' ') << text << endl;
    }

    // 5-row block letters made of '#'
    static vector<string> glyph(char ch) {
        switch (ch) {
        case 'A': return { " ### ", "#   #", "#####", "#   #", "#   #" };
        case 'B': return { "#### ", "#   #", "#### ", "#   #", "#### " };
        case 'E': return { "#####", "#    ", "#### ", "#    ", "#####" };
        case 'K': return { "#   #", "#  # ", "###  ", "#  # ", "#   #" };
        case 'L': return { "#    ", "#    ", "#    ", "#    ", "#####" };
        case 'M': return { "#   #", "## ##", "# # #", "#   #", "#   #" };
        case 'N': return { "#   #", "##  #", "# # #", "#  ##", "#   #" };
        case 'O': return { " ### ", "#   #", "#   #", "#   #", " ### " };
        case 'R': return { "#### ", "#   #", "#### ", "#  # ", "#   #" };
        case 'S': return { " ####", "#    ", " ### ", "    #", "#### " };
        case 'T': return { "#####", "  #  ", "  #  ", "  #  ", "  #  " };
        case 'U': return { "#   #", "#   #", "#   #", "#   #", " ### " };
        default:  return { "     ", "     ", "     ", "     ", "     " };   // space / unknown
        }
    }

    static void bigText(const string& text) {
        for (int row = 0; row < 5; row++) {
            string line;
            for (char ch : text) {
                line += glyph((char)toupper((unsigned char)ch))[row];
                line += " ";
            }
            center(line);
        }
    }

public:
    // big ASCII-art bank name (change the text here to rename the bank)
    // supported letters: A B E K L M N O R S T U and space
    static void bankName() {
        cout << UI::CLR_GOLD;
        bigText("SMART");
        cout << endl;
        bigText("BANK");
        cout << UI::CLR_RESET;
    }

    static void welcome() {
        for (int i = 0; i < 5; i++) cout << endl;
        bankName();
        cout << endl;
        cout << UI::CLR_GOLD;
        center("- - - - - - - - - - - - - - - - - - -");
        cout << UI::CLR_WHITE;
        center("WELCOME TO OUR BANK");
        cout << UI::CLR_GOLD;
        center("- - - - - - - - - - - - - - - - - - -");
        cout << UI::CLR_GRAY;
        center("Safe  |  Trusted  |  Yours");
        cout << UI::CLR_RESET << endl;
    }

    // small header shown on top of the login screen
    static void title() {
        cout << UI::CLR_GOLD;
        center("================================");
        cout << UI::CLR_WHITE;
        center("SMART BANK SYSTEM");
        cout << UI::CLR_GOLD;
        center("================================");
        cout << UI::CLR_RESET << endl;
    }

    static void loginOptions() {
        cout << "(1) Admin" << endl;
        cout << "(2) Employee" << endl;
        cout << "(3) Client\n" << endl;
    }

    static int loginAs() {
        int choice;
        if (!UI::readInt("Login as: ", choice)) return -1;
        return choice;
    }

    static void invalid(int c) {
        cout << "Invalid option: " << c << ". Please try again." << endl;
    }

    static void logout() {
        cout << "\nYou have been logged out." << endl;
        UI::pause();
    }

    // c: 1 = Admin, 2 = Employee, 3 = Client
    static void loginScreen(int c) {
        int id;
        if (!UI::readInt("Enter ID: ", id)) { UI::pause(); return; }
        string password = UI::readLine("Enter password: ");

        switch (c) {
        case 1: {
            Admin* admin = AdminManager::login(id, password);
            if (admin == nullptr) { cout << "Wrong ID or password." << endl; UI::pause(); return; }
            UI::clearScreen();
            UI::showInfo(admin);
            if (!optionsOrExit()) { exitApp = true; return; }
            while (AdminManager::AdminOptions(admin));
            logout();
            break;
        }
        case 2: {
            Employee* emp = EmployeeManager::login(id, password);
            if (emp == nullptr) { cout << "Wrong ID or password." << endl; UI::pause(); return; }
            UI::clearScreen();
            UI::showInfo(emp);
            if (!optionsOrExit()) { exitApp = true; return; }
            while (EmployeeManager::employeeOptions(emp));
            logout();
            break;
        }
        case 3: {
            Client* client = ClientManager::login(id, password);
            if (client == nullptr) { cout << "Wrong ID or password." << endl; UI::pause(); return; }
            UI::clearScreen();
            UI::showInfo(client);
            if (!optionsOrExit()) { exitApp = true; return; }
            while (ClientManager::clientOptions(client));
            logout();
            break;
        }
        default:
            invalid(c);
        }
    }

    static void runApp() {
        cout << fixed << setprecision(2);   // money is always shown with 2 decimals
        FileManager fm;
        fm.getAllClients();
        fm.getAllEmployees();
        fm.getAllAdmins();

        if (storage.Admins.empty()) {
            firstRunSetup(fm);
        }

        UI::setTheme();
        UI::clearScreen();
        welcome();
        UI::pause();

        while (!exitApp) {
            UI::clearScreen();
            title();
            loginOptions();

            int choice = loginAs();
            if (choice >= 1 && choice <= 3) {
                loginScreen(choice);
            }
            else {
                invalid(choice);
                UI::pause();
            }
        }
        cout << "\nGoodbye!" << endl;
    }
};