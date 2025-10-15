#include "PublicRelationsAdmin.h"
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

void PublicRelationsAdmin::display() const {
    cout << "\n=== Public Relations Admin Details ===" << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Role: " << toString(getAdminType()) << endl;
}

//Create notice
void PublicRelationsAdmin::createNotice() {
    string title, message;

    cout << "\n=== CREATE NEW NOTICE ===" << endl;
    cout << string(70, '=') << endl;

    cout << "Enter notice title: ";
    cin.ignore();
    getline(cin, title);

    if (title.empty()) {
        cout << "\nError: Title cannot be empty!" << endl;
        return;
    }

    cout << "Enter notice message: ";
    getline(cin, message);

    if (message.empty()) {
        cout << "\nError: Message cannot be empty!" << endl;
        return;
    }

    //current date use kore new notice creation
    Date currentDate = Date::getCurrentDate();
    Notice newNotice(title, message, currentDate);

    if (DatabaseManager::addNotice(newNotice)) {
        cout << "\nSuccess: Notice created successfully!" << endl;
        cout << "   Date: " << currentDate.toString() << endl;
    } else {
        cout << "\nError: Failed to create notice." << endl;
    }
}

//existing notice gula dekhay
void PublicRelationsAdmin::viewAllNotices() const {
    vector<Notice> notices = DatabaseManager::loadNotices();

    if (notices.empty()) {
        cout << "\n Info: No notices available." << endl;
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

//Deleting notice
void PublicRelationsAdmin::deleteNotice() {
    vector<Notice> notices = DatabaseManager::loadNotices();

    if (notices.empty()) {
        cout << "\n Info: No notices to delete." << endl;
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
        cout << "\nError: Invalid notice number!" << endl;
        return;
    }

    cout << "\n Warning: Are you sure you want to delete this notice? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        notices.erase(notices.begin() + (choice - 1));
        DatabaseManager::saveNotices(notices);
        cout << "\nSuccess: Notice deleted successfully!" << endl;
    } else {
        cout << "\nInfo: Deletion cancelled." << endl;
    }
}

//View notice
void PublicRelationsAdmin::viewNoticeDetails() const {
    vector<Notice> notices = DatabaseManager::loadNotices();

    if (notices.empty()) {
        cout << "\nInfo: No notices available." << endl;
        return;
    }


    viewAllNotices();

    cout << "\nEnter notice number to view details (0 to cancel): ";
    int choice;
    cin >> choice;

    if (choice == 0) {
        return;
    }

    if (choice < 1 || choice > static_cast<int>(notices.size())) {
        cout << "\nError: Invalid notice number!" << endl;
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


//Display statistics
void PublicRelationsAdmin::displayNoticeStatistics() const {
    vector<Notice> notices = DatabaseManager::loadNotices();

    cout << "\n=== NOTICE STATISTICS ===" << endl;
    cout << string(70, '=') << endl;
    cout << "Total Notices: " << notices.size() << endl;

    if (!notices.empty()) {
        //Month/year onujayi notice count kore
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

        //last 7 din er notice count kore
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
