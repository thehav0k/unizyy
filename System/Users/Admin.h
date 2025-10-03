#ifndef ADMIN_H
#define ADMIN_H
#include "user.h"
#include <vector>
#include "../../Core/Models/admintype.h"
#include "../../Core/Utils/StringHelper.h"
#include "../../Core/Database/DatabaseManager.h"


class Admin: public User {
private:
    char name[100];
    AdminType adminType;

public:
    // constructors // setters // getters
    Admin(const string &email, const string &password, const string &name, AdminType adminType)
        : User(email, password), adminType(adminType) {
        StringHelper::stringToCharArray(name, this->name);
    }
    Admin() : User(), adminType(AdminType::SystemAdmin) {
        name[0] = '\0';
    }

    string getName() const {
        return StringHelper::charArrayToString(name);
    }

    void setName(const string &name) {
        StringHelper::stringToCharArray(name, this->name);
    }

    AdminType getAdminType() const {
        return adminType;
    }

    void setAdminType(AdminType adminType) {
        this->adminType = adminType;
    }

    void display() const override {
        cout << "Admin Details:" << endl;
        cout << "Name: " << getName() << endl;
        cout << "Email: " << getEmail() << endl;
        cout << "Admin Type: " << static_cast<int>(adminType) << endl;
    }

    // Database er sob kaj Database Manager korbe
    static vector<Admin> loadAllAdmins();
    static bool addAdminToDB(const Admin& admin);
    static bool updateAdminInDB(const string& email, const Admin& updatedAdmin);
    static bool deleteAdminFromDB(const string& email);
    static Admin* findAdminByEmail(const string& email);
};


#endif //ADMIN_H
