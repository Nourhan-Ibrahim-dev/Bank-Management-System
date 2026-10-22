#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <cctype>
using namespace std;

class Validation
{
public:
    static bool validateName(string name)
    {
        if (name.length() < 3 || name.length() > 20) {
            return false;
        }
        for (size_t i = 0; i < name.length(); i++) {
            if (!((name[i] >= 'A' && name[i] <= 'Z') ||
                (name[i] >= 'a' && name[i] <= 'z') ||
                name[i] == ' ')) {
                return false;
            }
        }
        return true;
    }

    // 8-20 chars, no whitespace, and no '|' (it is the separator in the data files,
    // a password containing it would corrupt the saved line and lose the account)
    static bool validatePassword(string password)
    {
        if (password.length() < 8 || password.length() > 20) {
            return false;
        }
        for (size_t i = 0; i < password.length(); i++) {
            unsigned char ch = (unsigned char)password[i];
            if (isspace(ch) || ch == '|') {
                return false;
            }
        }
        return true;
    }

    static bool validateSalary(double salary)
    {
        return isfinite(salary) && salary >= 5000;
    }

    static bool validateBalance(double balance)
    {
        return isfinite(balance) && balance >= 1500;
    }
};