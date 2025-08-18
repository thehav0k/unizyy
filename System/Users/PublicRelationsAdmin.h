// PublicRelationsAdmin class header (simplified after notice removal)
#pragma once

#include "user.h"
#include "../../Core/Models/department.h"
#include "../../Core/Utils/StringHelper.h"
#include <string>
#include <iostream>

class PublicRelationsAdmin : public User {
private:
    char name[100];
    char adminID[40];
    department managedDepartment;

public:
    // Constructors
    PublicRelationsAdmin() : User() {
        name[0] = '\0';
        adminID[0] = '\0';
        managedDepartment = department::Department_of_Computer_Science_and_Engineering;
    }
    PublicRelationsAdmin(const std::string& n, const std::string& email, const std::string& password,
                         const std::string& id, department dept)
        : User(email, password), managedDepartment(dept) {
        StringHelper::stringToCharArray(n, name);
        StringHelper::stringToCharArray(id, adminID);
    }

    // Getters / Setters
    std::string getName() const { return StringHelper::charArrayToString(name); }
    void setName(const std::string& n) { StringHelper::stringToCharArray(n, name); }
    std::string getAdminID() const { return StringHelper::charArrayToString(adminID); }
    void setAdminID(const std::string& id) { StringHelper::stringToCharArray(id, adminID); }
    department getManagedDepartment() const { return managedDepartment; }
    void setManagedDepartment(department dept) { managedDepartment = dept; }

    // Display
    void display() const override {
        std::cout << "PR Admin: " << getName() << " (" << getEmail() << ")\n";
    }
};
