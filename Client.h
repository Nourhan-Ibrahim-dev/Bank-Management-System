#pragma once
#include "Person.h"

class Client : public Person
{
protected:
    double balance;
    static constexpr double MIN_BALANCE = 1500;

public:
    // Used when reading the client from the file:
    // the id already exists, so we keep the real id and don't generate a new one
    Client(int id, string name, string password, double balance) : Person(id, name, password) {
        this->balance = balance;
    }

    void setBalance(double balance) {
        if (Validation::validateBalance(balance)) {
            this->balance = balance;
        }
        else {
            cout << "invalid balance " << endl;
        }
    }

    double getBalance() {
        return balance;
    }

    void deposit(double amount) {
        if (!isfinite(amount) || amount <= 0) {
            cout << "Invalid amount" << endl;
            return;
        }
        balance += amount;
    }

    // the balance can never drop below the minimum balance (same rule as account creation)
    void withdraw(double amount) {
        if (!isfinite(amount) || amount <= 0) {
            cout << "Invalid amount" << endl;
            return;
        }
        if (balance - amount < MIN_BALANCE) {
            cout << "Cant withdraw ! the balance must stay at least " << MIN_BALANCE << "." << endl;
            return;
        }
        balance = balance - amount;
        cout << "withdraw is done !" << endl;
    }

    void transferTo(double amount, Client& recipient) {
        if (!isfinite(amount) || amount <= 0) {
            cout << "Invalid amount" << endl;
            return;
        }
        if (balance - amount < MIN_BALANCE) {
            cout << "Cant transfer ! the balance must stay at least " << MIN_BALANCE << "." << endl;
            return;
        }
        balance = balance - amount;
        recipient.balance += amount;
        cout << "Transfer was a success!" << endl;
    }

    void checkBalance() {
        cout << "Current balance is : " << balance << endl;
    }

    void displayDetails() override {
        Person::displayDetails();
        cout << "Balance : " << balance << endl;
    }
};