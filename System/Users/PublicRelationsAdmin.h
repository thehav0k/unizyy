//
// Created by Md. Asif Khan on 12/10/25.
//

#ifndef PUBLICRELATIONSADMIN_H
#define PUBLICRELATIONSADMIN_H

#include "Admin.h"
#include "../../Core/Models/Notice.h"
#include "../../Core/Models/date.h"
#include "../../Core/Utils/StringHelper.h"
#include "../../Core/Database/DatabaseManager.h"
#include <string>
#include <vector>

using namespace std;

class PublicRelationsAdmin : public Admin {
private:
    int age;

public:
    // Constructors
    PublicRelationsAdmin(const string& email, const string& password,
                        const string& name, int age)
        : Admin(email, password, name, AdminType::PublicRelations), age(age) {
    }

    // Default constructor for binary I/O
    PublicRelationsAdmin() : Admin(), age(0) {
        setAdminType(AdminType::PublicRelations);
    }

    // Getters and Setters
    int getAge() const { return age; }
    void setAge(int age) { this->age = age; }

    // Display override
    void display() const override;

    // Notice Management Operations (interactive menu-driven)
    void createNotice();
    void viewAllNotices() const;
    void editNotice();
    void deleteNotice();
    void viewNoticeDetails() const;
    void searchNotices() const;
    void displayNoticeStatistics() const;

    // Database operations (delegates to DatabaseManager like Student does)
    static vector<Notice> loadAllNotices();
    static bool addNoticeToDB(const Notice& notice);
    static bool updateNoticeInDB(int index, const Notice& updatedNotice);
    static void saveNoticesToDB(const vector<Notice>& notices);
};

#endif // PUBLICRELATIONSADMIN_H

