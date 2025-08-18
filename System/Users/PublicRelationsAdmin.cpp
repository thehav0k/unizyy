//
// Created by Md. Asif Khan on 15/8/25.
//

#include "PublicRelationsAdmin.h"
#include "../../Core/Utils/StringHelper.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Constructors
PublicRelationsAdmin::PublicRelationsAdmin() : User(), NoticeManager() {
    adminID = "";
    managedDepartment = department::GENERAL;
}

PublicRelationsAdmin::PublicRelationsAdmin(const string& name, const string& email, const string& password,
                                          const string& adminID, department dept)
    : User(name, email, password), NoticeManager() {
    this->adminID = adminID;
    this->managedDepartment = dept;
}

// Getters
string PublicRelationsAdmin::getAdminID() const {
    return adminID;
}

department PublicRelationsAdmin::getManagedDepartment() const {
    return managedDepartment;
}

// Setters
void PublicRelationsAdmin::setAdminID(const string& id) {
    adminID = id;
}

void PublicRelationsAdmin::setManagedDepartment(department dept) {
    managedDepartment = dept;
}

// Notice management methods specific to PR Admin
bool PublicRelationsAdmin::postNotice(const string& title, const string& description, NoticeType type) {
    Notice newNotice(title, description, type);
    newNotice.setTargetDepartment(managedDepartment);
    return saveNotice(newNotice);
}

bool PublicRelationsAdmin::postDepartmentNotice(const string& title, const string& description, NoticeType type) {
    Notice newNotice(title, description, type);
    newNotice.setTargetDepartment(managedDepartment);
    return saveNotice(newNotice);
}

bool PublicRelationsAdmin::postGeneralNotice(const string& title, const string& description) {
    Notice newNotice(title, description, NoticeType::GENERAL);
    newNotice.setTargetDepartment(department::GENERAL);
    return saveNotice(newNotice);
}

void PublicRelationsAdmin::viewAllNotices() const {
    vector<Notice> notices = getAllNotices();
    cout << "\n=== All Notices ===" << endl;

    if (notices.empty()) {
        cout << "No notices found." << endl;
        return;
    }

    for (const auto& notice : notices) {
        notice.displayNotice();
        cout << "------------------------" << endl;
    }
}

void PublicRelationsAdmin::viewNoticesByType(NoticeType type) const {
    displayNoticesByType(type);
}

void PublicRelationsAdmin::viewDepartmentNotices() const {
    vector<Notice> notices = getNoticesByDepartment(managedDepartment);
    cout << "\n=== Department Notices ===" << endl;

    if (notices.empty()) {
        cout << "No department notices found." << endl;
        return;
    }

    for (const auto& notice : notices) {
        notice.displayNotice();
        cout << "------------------------" << endl;
    }
}

void PublicRelationsAdmin::viewMyNotices() const {
    vector<Notice> notices = getMyPublishedNotices();
    cout << "\n=== My Published Notices ===" << endl;

    if (notices.empty()) {
        cout << "You haven't published any notices yet." << endl;
        return;
    }

    for (const auto& notice : notices) {
        notice.displayNotice();
        cout << "------------------------" << endl;
    }
}

// Enhanced notice operations
bool PublicRelationsAdmin::editNotice(size_t noticeID, const string& newTitle, const string& newDescription) {
    Notice* notice = findNoticeByID(noticeID);
    if (!notice) {
        cout << "Notice not found." << endl;
        return false;
    }

    notice->setTitle(newTitle);
    notice->setDescription(newDescription);
    return updateNoticeById(noticeID, *notice, getEmail());
}

bool PublicRelationsAdmin::deleteNotice(size_t noticeID) {
    return removeNoticeById(noticeID, getEmail());
}

bool PublicRelationsAdmin::changeNoticeType(size_t noticeID, NoticeType newType) {
    Notice* notice = findNoticeByID(noticeID);
    if (!notice) {
        cout << "Notice not found." << endl;
        return false;
    }

    notice->setNoticeType(newType);
    return updateNoticeById(noticeID, *notice, getEmail());
}

// Search and filter operations
void PublicRelationsAdmin::searchNoticesByKeyword(const string& keyword) const {
    vector<Notice> results = searchNotices(keyword);
    cout << "\n=== Search Results for '" << keyword << "' ===" << endl;

    if (results.empty()) {
        cout << "No notices found matching the keyword." << endl;
        return;
    }

    for (const auto& notice : results) {
        notice.displayNotice();
        cout << "------------------------" << endl;
    }
}

void PublicRelationsAdmin::displayNoticeStatistics() const {
    cout << "\n=== Notice Statistics ===" << endl;
    cout << "Total Notices: " << getTotalNoticesCount() << endl;
    cout << "Active Notices: " << getActiveNoticesCount() << endl;

    auto stats = getNoticeStatsByType();
    cout << "\nNotices by Type:" << endl;
    for (const auto& stat : stats) {
        cout << NoticeTypeHelper::toString(stat.first) << ": " << stat.second << endl;
    }
}

// Override from NoticeManager for PR-specific functionality
vector<Notice> PublicRelationsAdmin::getMyPublishedNotices() const {
    // Return notices for this admin's department or all notices they can manage
    return getNoticesByDepartment(managedDepartment);
}

// Display methods
void PublicRelationsAdmin::displayMenu() const {
    cout << "\n=== Public Relations Admin Menu ===" << endl;
    cout << "1. Notice Management" << endl;
    cout << "2. View Statistics" << endl;
    cout << "3. Search Notices" << endl;
    cout << "4. View Profile" << endl;
    cout << "5. Logout" << endl;
    cout << "Enter your choice: ";
}

void PublicRelationsAdmin::displayNoticeManagementMenu() const {
    cout << "\n=== Notice Management Menu ===" << endl;
    cout << "1. Create New Notice" << endl;
    cout << "2. View All Notices" << endl;
    cout << "3. View Notices by Type" << endl;
    cout << "4. View Department Notices" << endl;
    cout << "5. Edit Notice" << endl;
    cout << "6. Delete Notice" << endl;
    cout << "7. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
}

// User interface methods
void PublicRelationsAdmin::handleNoticeCreation() {
    string title, description, typeStr;

    cout << "\n=== Create New Notice ===" << endl;

    displayAllNoticeTypes();
    cout << "\nEnter notice type: ";
    getline(cin, typeStr);

    if (!NoticeTypeHelper::isValidNoticeType(typeStr)) {
        cout << "Invalid notice type!" << endl;
        return;
    }

    cout << "Enter notice title: ";
    getline(cin, title);

    cout << "Enter notice description: ";
    getline(cin, description);

    NoticeType type = NoticeTypeHelper::fromString(typeStr);
    if (postNotice(title, description, type)) {
        cout << "Notice created successfully!" << endl;
    } else {
        cout << "Failed to create notice." << endl;
    }
}

void PublicRelationsAdmin::handleNoticeViewing() {
    int choice;
    cout << "\n=== View Notices ===" << endl;
    cout << "1. View All Notices" << endl;
    cout << "2. View by Type" << endl;
    cout << "3. View Department Notices" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1:
            viewAllNotices();
            break;
        case 2: {
            displayAllNoticeTypes();
            string typeStr;
            cout << "Enter notice type: ";
            getline(cin, typeStr);
            if (NoticeTypeHelper::isValidNoticeType(typeStr)) {
                viewNoticesByType(NoticeTypeHelper::fromString(typeStr));
            } else {
                cout << "Invalid notice type!" << endl;
            }
            break;
        }
        case 3:
            viewDepartmentNotices();
            break;
        default:
            cout << "Invalid choice!" << endl;
    }
}

void PublicRelationsAdmin::handleNoticeEditing() {
    size_t noticeID;
    string newTitle, newDescription;

    cout << "\n=== Edit Notice ===" << endl;
    cout << "Enter notice ID: ";
    cin >> noticeID;
    cin.ignore();

    Notice* notice = findNoticeByID(noticeID);
    if (!notice) {
        cout << "Notice not found!" << endl;
        return;
    }

    cout << "Current title: " << notice->getTitle() << endl;
    cout << "Enter new title (or press Enter to keep current): ";
    getline(cin, newTitle);
    if (newTitle.empty()) {
        newTitle = notice->getTitle();
    }

    cout << "Current description: " << notice->getDescription() << endl;
    cout << "Enter new description (or press Enter to keep current): ";
    getline(cin, newDescription);
    if (newDescription.empty()) {
        newDescription = notice->getDescription();
    }

    if (editNotice(noticeID, newTitle, newDescription)) {
        cout << "Notice updated successfully!" << endl;
    } else {
        cout << "Failed to update notice." << endl;
    }
}

void PublicRelationsAdmin::handleNoticeSearch() {
    string keyword;
    cout << "\n=== Search Notices ===" << endl;
    cout << "Enter keyword to search: ";
    getline(cin, keyword);

    searchNoticesByKeyword(keyword);
}
