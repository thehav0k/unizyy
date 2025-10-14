
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
public:
    //Constructors
    PublicRelationsAdmin(const string& email, const string& password,
                        const string& name)
        : Admin(email, password, name, AdminType::PublicRelations) {
    }

    //Binary I/O er jonno default constructor(file theke read/write er shomoy use hoy)
    PublicRelationsAdmin() : Admin() {
        setAdminType(AdminType::PublicRelations);
    }

    void display() const override;

    //Notice Management Operations
    void createNotice();
    void viewAllNotices() const;
    void editNotice();
    void deleteNotice();
    void viewNoticeDetails() const;
    void searchNotices() const;
    void displayNoticeStatistics() const;

    //Database operations
    static vector<Notice> loadAllNotices();
    static bool addNoticeToDB(const Notice& notice);
    static bool updateNoticeInDB(int index, const Notice& updatedNotice);
    static void saveNoticesToDB(const vector<Notice>& notices);
};

#endif // PUBLICRELATIONSADMIN_H

