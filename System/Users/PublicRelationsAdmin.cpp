//
// Created by Md. Asif Khan on 12/10/25.
//

#include "PublicRelationsAdmin.h"
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

void PublicRelationsAdmin::display() const {
    cout << "\n=== Public Relations Admin Details ===" << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Age: " << age << endl;
    cout << "Role: " << toString(getAdminType()) << endl;
}

// Create notice (interactive menu-driven like Student::BuyMealToken)
void PublicRelationsAdmin::createNotice() {
    string title, message;

    cout << "\n=== CREATE NEW NOTICE ===" << endl;
    cout << string(70, '=') << endl;

    cout << "Enter notice title: ";
    cin.ignore();
    getline(cin, title);

    if (title.empty()) {
        cout << "\n✗ Error: Title cannot be empty!" << endl;
        return;
    }

    cout << "Enter notice message: ";
    getline(cin, message);

    if (message.empty()) {
        cout << "\n✗ Error: Message cannot be empty!" << endl;
        return;
    }

    // Use current date from Date class
    Date currentDate = Date::getCurrentDate();
    Notice newNotice(title, message, currentDate);

    if (DatabaseManager::addNotice(newNotice)) {
        cout << "\n✓ Success: Notice created successfully!" << endl;
        cout << "   Date: " << currentDate.toString() << endl;
    } else {
        cout << "\n✗ Error: Failed to create notice." << endl;
    }
}

// View all notices
void PublicRelationsAdmin::viewAllNotices() const {
    vector<Notice> notices = DatabaseManager::loadNotices();

    if (notices.empty()) {
        cout << "\nℹ Info: No notices available." << endl;
        return;
    }

    cout << "\n=== ALL NOTICES ===" << endl;
    cout << string(70, '=') << endl;

    for (size_t i = 0; i < notices.size(); i++) {
        cout << "\n[" << (i + 1) << "] " << notices[i].getTitle() << endl;
        cout << "    Date: " << notices[i].getDate().toString() << endl;
        cout << string(70, '-') << endl;
    }

    cout << "\nTotal Notices: " << notices.size() << endl;
}

// Edit existing notice
void PublicRelationsAdmin::editNotice() {
    vector<Notice> notices = DatabaseManager::loadNotices();

    if (notices.empty()) {
        cout << "\nℹ Info: No notices to edit." << endl;
        return;
    }

    // Show all notices first
    viewAllNotices();

    cout << "\nEnter notice number to edit (0 to cancel): ";
    int choice;
    cin >> choice;

    if (choice == 0) {
        cout << "Operation cancelled." << endl;
        return;
    }

    if (choice < 1 || choice > static_cast<int>(notices.size())) {
        cout << "\n✗ Error: Invalid notice number!" << endl;
        return;
    }

    int index = choice - 1;
    Notice& notice = notices[index];

    cout << "\n=== EDIT NOTICE ===" << endl;
    cout << string(70, '=') << endl;
    cout << "Current Title: " << notice.getTitle() << endl;
    cout << "Current Message: " << notice.getMessage() << endl;

    cin.ignore();
    cout << "\nEnter new title (or press Enter to keep current): ";
    string newTitle;
    getline(cin, newTitle);
    if (!newTitle.empty()) {
        notice.setTitle(newTitle);
    }

    cout << "Enter new message (or press Enter to keep current): ";
    string newMessage;
    getline(cin, newMessage);
    if (!newMessage.empty()) {
        notice.setMessage(newMessage);
    }

    // Update date to current date
    notice.setDate(Date::getCurrentDate());

    if (DatabaseManager::updateNotice(index, notice)) {
        cout << "\n✓ Success: Notice updated successfully!" << endl;
        cout << "   Updated on: " << Date::getCurrentDate().toString() << endl;
    } else {
        cout << "\n✗ Error: Failed to update notice." << endl;
    }
}

// Delete notice
void PublicRelationsAdmin::deleteNotice() {
    vector<Notice> notices = DatabaseManager::loadNotices();

    if (notices.empty()) {
        cout << "\nℹ Info: No notices to delete." << endl;
        return;
    }

    // Show all notices first
    viewAllNotices();

    cout << "\nEnter notice number to delete (0 to cancel): ";
    int choice;
    cin >> choice;

    if (choice == 0) {
        cout << "Operation cancelled." << endl;
        return;
    }

    if (choice < 1 || choice > static_cast<int>(notices.size())) {
        cout << "\n✗ Error: Invalid notice number!" << endl;
        return;
    }

    cout << "\n⚠  Warning: Are you sure you want to delete this notice? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        notices.erase(notices.begin() + (choice - 1));
        DatabaseManager::saveNotices(notices);
        cout << "\n✓ Success: Notice deleted successfully!" << endl;
    } else {
        cout << "\nℹ Info: Deletion cancelled." << endl;
    }
}

// View detailed notice
void PublicRelationsAdmin::viewNoticeDetails() const {
    vector<Notice> notices = DatabaseManager::loadNotices();

    if (notices.empty()) {
        cout << "\nℹ Info: No notices available." << endl;
        return;
    }

    // Show all notices first
    viewAllNotices();

    cout << "\nEnter notice number to view details (0 to cancel): ";
    int choice;
    cin >> choice;

    if (choice == 0) {
        return;
    }

    if (choice < 1 || choice > static_cast<int>(notices.size())) {
        cout << "\n✗ Error: Invalid notice number!" << endl;
        return;
    }

    const Notice& notice = notices[choice - 1];

    cout << "\n" << string(70, '=') << endl;
    cout << "NOTICE DETAILS" << endl;
    cout << string(70, '=') << endl;
    cout << "\nTitle: " << notice.getTitle() << endl;
    cout << "Date: " << notice.getDate().toString() << endl;
    cout << string(70, '-') << endl;
    cout << "\nMessage:\n" << notice.getMessage() << endl;
    cout << "\n" << string(70, '=') << endl;
}

// Search notices by keyword
void PublicRelationsAdmin::searchNotices() const {
    cout << "\nEnter search keyword: ";
    string keyword;
    cin.ignore();
    getline(cin, keyword);

    if (keyword.empty()) {
        cout << "\n✗ Error: Search keyword cannot be empty!" << endl;
        return;
    }

    vector<Notice> notices = DatabaseManager::loadNotices();
    vector<Notice> results;

    // Case-insensitive search in title
    transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

    for (const auto& notice : notices) {
        string title = notice.getTitle();
        string message = notice.getMessage();

        // Convert to lowercase for comparison
        transform(title.begin(), title.end(), title.begin(), ::tolower);
        transform(message.begin(), message.end(), message.begin(), ::tolower);

        // Search in both title and message
        if (title.find(keyword) != string::npos || message.find(keyword) != string::npos) {
            results.push_back(notice);
        }
    }

    if (results.empty()) {
        cout << "\nℹ Info: No matching notices found." << endl;
        return;
    }

    cout << "\n=== SEARCH RESULTS ===" << endl;
    cout << string(70, '=') << endl;

    for (size_t i = 0; i < results.size(); i++) {
        cout << "\n[" << (i + 1) << "] " << results[i].getTitle() << endl;
        cout << "    Date: " << results[i].getDate().toString() << endl;
        cout << string(70, '-') << endl;
    }

    cout << "\nFound " << results.size() << " matching notice(s)." << endl;
}

// Display statistics
void PublicRelationsAdmin::displayNoticeStatistics() const {
    vector<Notice> notices = DatabaseManager::loadNotices();

    cout << "\n=== NOTICE STATISTICS ===" << endl;
    cout << string(70, '=') << endl;
    cout << "Total Notices: " << notices.size() << endl;

    if (!notices.empty()) {
        // Group by month-year
        map<string, int> monthlyCount;

        for (const auto& notice : notices) {
            Date d = notice.getDate();
            string monthKey = to_string(d.getMonth()) + "/" + to_string(d.getYear());
            monthlyCount[monthKey]++;
        }

        cout << "\nNotices by Month:" << endl;
        cout << string(70, '-') << endl;

        for (const auto& pair : monthlyCount) {
            cout << "  " << pair.first << ": " << pair.second << " notice(s)" << endl;
        }

        // Recent notices (last 7 days)
        Date today = Date::getCurrentDate();
        int recentCount = 0;

        for (const auto& notice : notices) {
            if (notice.getDate() >= today.getPreviousDay().getPreviousDay().getPreviousDay()
                .getPreviousDay().getPreviousDay().getPreviousDay().getPreviousDay()) {
                recentCount++;
            }
        }

        cout << "\nRecent Notices (Last 7 days): " << recentCount << endl;
    }

    cout << string(70, '=') << endl;
}

// Static database delegates (following Student pattern)
vector<Notice> PublicRelationsAdmin::loadAllNotices() {
    return DatabaseManager::loadNotices();
}

bool PublicRelationsAdmin::addNoticeToDB(const Notice& notice) {
    return DatabaseManager::addNotice(notice);
}

bool PublicRelationsAdmin::updateNoticeInDB(int index, const Notice& updatedNotice) {
    return DatabaseManager::updateNotice(index, updatedNotice);
}

void PublicRelationsAdmin::saveNoticesToDB(const vector<Notice>& notices) {
    DatabaseManager::saveNotices(notices);
}

