//
// Created by Md. Asif Khan on 15/8/25.
//

#include "PublicRelationsAdmin.h"
#include "../../Core/Utils/StringHelper.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Constructors
PublicRelationsAdmin::PublicRelationsAdmin() : User(), NoticeManager("pr_notices") {
    name[0] = '\0';
}

PublicRelationsAdmin::PublicRelationsAdmin(const string& email, const string& password, const string& name)
    : User(email, password), NoticeManager("pr_notices") {
    StringHelper::stringToCharArray(name, this->name);
}

// Basic getters and setters
string PublicRelationsAdmin::getName() const {
    return StringHelper::charArrayToString(name);
}

void PublicRelationsAdmin::setName(const string& n) {
    StringHelper::stringToCharArray(n, this->name);
}

// Virtual methods from User
void PublicRelationsAdmin::display() const {
    cout << "Public Relations Admin Details:" << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Total Notices Published: " << getMyPublishedNotices().size() << endl;
    cout << "Active Notices: " << getActiveNoticesCount() << endl;
}

// Notice creation methods (simplified)
Notice PublicRelationsAdmin::createNotice(const string& title, const string& description, NoticeType type) const {
    Notice notice(title, description, type, getName(), getEmail());
    notice.setTargetAudience("All");
    return notice;
}

Notice PublicRelationsAdmin::createPublicAnnouncement(const string& title, const string& description) const {
    Notice notice(title, description, NoticeType::GENERAL, getName(), getEmail());
    notice.setTargetAudience("All");
    return notice;
}

// Notice management operations (simplified)
bool PublicRelationsAdmin::publishNotice(const Notice& notice) const {
    return saveNotice(notice);
}

bool PublicRelationsAdmin::publishNotice(const string& title, const string& body) const {
    Notice notice = createPublicAnnouncement(title, body);
    return publishNotice(notice);
}

// Bulk operations
bool PublicRelationsAdmin::publishMultipleNotices(const vector<Notice>& notices) const {
    vector<Notice> allNotices = getAllNotices();

    for (const auto& notice : notices) {
        allNotices.push_back(notice);
    }

    DatabaseManager::saveObjects(allNotices, noticeDbFile);
    return true;
}

bool PublicRelationsAdmin::archiveOldNotices(const Date& cutoffDate) const {
    vector<Notice> allNotices = getAllNotices();
    vector<Notice> currentNotices;

    for (const auto& notice : allNotices) {
        if (notice.getCreatedDate() >= cutoffDate) {
            currentNotices.push_back(notice);
        }
    }

    if (currentNotices.size() != allNotices.size()) {
        DatabaseManager::saveObjects(currentNotices, noticeDbFile);
        return true;
    }
    return false;
}

// Enhanced search and filtering
vector<Notice> PublicRelationsAdmin::getNoticesForDepartment(department dept) const {
    return filterNotices([dept](const Notice& notice) {
        return notice.getTargetDepartment() == dept;
    });
}

void PublicRelationsAdmin::displayNoticeStatistics() const {
    cout << "\n=== Notice Statistics ===" << endl;
    cout << "Total Notices: " << getTotalNoticesCount() << endl;
    cout << "Active Notices: " << getActiveNoticesCount() << endl;
    cout << "My Published Notices: " << getMyPublishedNotices().size() << endl;

    auto stats = getNoticeStatsByType();
    cout << "\nNotices by Type:" << endl;
    for (const auto& stat : stats) {
        cout << "- " << NoticeTypeHelper::toString(stat.first) << ": " << stat.second << endl;
    }
}

// Static database operations
vector<PublicRelationsAdmin> PublicRelationsAdmin::loadAllPRAdmins() {
    return DatabaseManager::loadObjects<PublicRelationsAdmin>("pr_admins");
}

bool PublicRelationsAdmin::addPRAdminToDB(const PublicRelationsAdmin& admin) {
    vector<PublicRelationsAdmin> admins = loadAllPRAdmins();
    admins.push_back(admin);
    DatabaseManager::saveObjects(admins, "pr_admins");
    return true;
}

bool PublicRelationsAdmin::updatePRAdminInDB(const string& email, const PublicRelationsAdmin& updatedAdmin) {
    vector<PublicRelationsAdmin> admins = loadAllPRAdmins();

    for (auto& admin : admins) {
        if (admin.getEmail() == email) {
            admin = updatedAdmin;
            DatabaseManager::saveObjects(admins, "pr_admins");
            return true;
        }
    }
    return false;
}

bool PublicRelationsAdmin::deletePRAdminFromDB(const string& email) {
    vector<PublicRelationsAdmin> admins = loadAllPRAdmins();

    auto it = admins.begin();
    while (it != admins.end()) {
        if (it->getEmail() == email) {
            admins.erase(it);
            DatabaseManager::saveObjects(admins, "pr_admins");
            return true;
        } else {
            ++it;
        }
    }
    return false;
}

PublicRelationsAdmin* PublicRelationsAdmin::findPRAdminByEmail(const string& email) {
    static vector<PublicRelationsAdmin> admins = loadAllPRAdmins();

    for (auto& admin : admins) {
        if (admin.getEmail() == email) {
            return &admin;
        }
    }
    return nullptr;
}
