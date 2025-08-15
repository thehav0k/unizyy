//
// Created by Md. Asif Khan on 8/8/25.
//

#ifndef USER_H
#define USER_H
#include <string>
#include <iostream>
#include "../../Core/Utils/StringHelper.h"
using namespace std;

class User {
protected:
    char email[100];     // char array karon string diye object write kora jayna
    char password[32];   // 32 is enough I think

public:
    User(const string &email, const string &password) {
        StringHelper::stringToCharArray(email, this->email);
        StringHelper::stringToCharArray(password, this->password);
    }
// Default constructor
    User() {
        email[0] = '\0';
        password[0] = '\0';
    }

    string getEmail() const {
        return StringHelper::charArrayToString(email);
    }

    void setEmail(const string &email) {
        StringHelper::stringToCharArray(email, this->email);
    }

    string getPassword() const {
        return StringHelper::charArrayToString(password);
    }

    void setPassword(const string &password) {
        StringHelper::stringToCharArray(password, this->password);
    }

    // Password change functionality
    bool changePassword(const string& currentPassword, const string& newPassword) {
        if (getPassword() != currentPassword) {
            return false; // Current password doesn't match
        }
        setPassword(newPassword);
        return true;
    }

    // Virtual destructor for polymorphic classes
    virtual ~User() = default;

    // Pure virtual method for polymorphism
    virtual void display() const = 0;
};

#endif //USER_H
// check #1 by Asif