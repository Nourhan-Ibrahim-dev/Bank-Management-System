#pragma once
#include "Validation.h"

class Person {
private:
    inline static int nextId = 1; // C++17: definition lives in the header

protected:
    int id;
    string name;
    string password;

public:
    // id given manually (e.g. when loading from file)
    Person(int id, string name, string password) {
        this->id = id;
        this->name = name;
        this->password = password;
    }

    // id generated automatically (new person)
    Person(string name, string password) {
        id = nextId;
        nextId++;
        this->name = name;
        this->password = password;
    }

    virtual ~Person() {}

    void setName(string name) {
        // 3-20 letters, spaces allowed (see Validation::validateName)
        if (!Validation::validateName(name)) {
            cout << "Name should only include letters (3-20 chars)!" << endl;
            return;
        }
        this->name = name;
    }

    void setPassword(string password) {
        if (Validation::validatePassword(password)) {
            this->password = password;
        }
        else {
            cout << "invalid Password " << endl;
        }
    }

    void setId(int id) {
        this->id = id;
    }

    string getName() {
        return name;
    }

    string getPass() {
        return password;
    }

    int getId() {
        return id;
    }

    virtual void displayDetails() {
        cout << "Name : " << getName() << endl;
        cout << "Password : " << getPass() << endl;
    }
};