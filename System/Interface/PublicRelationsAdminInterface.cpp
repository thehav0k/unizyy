//
// Created by Md. Asif Khan on 15/8/25.
// Public Relations Admin Interface Implementation
//

#include "PublicRelationsAdminInterface.h"
#include "../Authentication/auth.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include "../../Core/Models/date.h"

using namespace std;

PublicRelationsAdminInterface::PublicRelationsAdminInterface(PublicRelationsAdmin* prAdmin, Auth* auth)
    : currentPRAdmin(prAdmin), authSystem(auth), isRunning(true) {}

void PublicRelationsAdminInterface::displayPRAdminDashboard() {
    clearScreen();
    displayHeader("PUBLIC RELATIONS ADMIN DASHBOARD");
    cout << "Welcome, " << currentPRAdmin->getName() << "!" << endl;
    cout << "Email: " << currentPRAdmin->getEmail() << endl;

    // Display quick stats
    auto myNotices = currentPRAdmin->getMyPublishedNotices();
    cout << "Published Notices: " << myNotices.size() << endl;
    cout << "Active Notices: " << currentPRAdmin->getActiveNoticesCount() << endl;
    cout << "Total System Notices: " << currentPRAdmin->getTotalNoticesCount() << endl;
    displaySeparator('-', 60);
}

void PublicRelationsAdminInterface::displayMenu() {
    displayPRAdminDashboard();

    cout << "0. Simulate Date" << endl;
    cout << "\nNOTICE MANAGEMENT MENU:" << endl;
    cout << "1. Create New Notice" << endl;
    cout << "2. Manage My Notices" << endl;
    cout << "3. Search & Browse Notices" << endl;
    cout << "4. Notice Statistics" << endl;
    cout << "5. Bulk Operations" << endl;
    cout << "6. Profile Settings" << endl;
    cout << "7. Logout" << endl;
    displaySeparator('-', 40);
}

int PublicRelationsAdminInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (0-7): ";

    while (!(cin >> choice) || choice < 0 || choice > 7) {
        displayError("Invalid input! Please enter a number between 0-7.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (0-7): ";
    }

    cin.ignore();
    return choice;
}

void PublicRelationsAdminInterface::processChoice(int choice) {
    switch (choice) {
        case 0: {
            displayHeader("SIMULATE DATE");
            cout << "Enter date (DD-MM-YYYY) or leave empty to reset: ";
            string input;
            getline(cin, input);
            if (input.empty()) {
                Date::resetSimulation();
                displaySuccess("Simulation reset to current date.");
            } else {
                Date d(input);
                if (!d.isValid()) {
                    displayError("Invalid date format.");
                } else {
                    Date::setSimulatedDate(d);
                    displaySuccess("Simulation date set to " + d.toString());
                }
            }
            pauseForInput();
        } break;
        case 1:
            handleNoticeCreation();
            break;
        case 2:
            handleNoticeManagement();
            break;
        case 3:
            handleNoticeSearch();
            break;
        case 4:
            handleNoticeStatistics();
            break;
        case 5:
            handleBulkOperations();
            break;
        case 6:
            handleProfile();
            break;
        case 7:
            handleLogout();
            break;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void PublicRelationsAdminInterface::handleNoticeCreation() {
    clearScreen();
    displayHeader("CREATE NEW NOTICE");

    cout << "1. Create Simple Notice" << endl;
    cout << "2. Create Detailed Notice" << endl;
    cout << "3. Back to Main Menu" << endl;

    int choice;
    cout << "Select option (1-3): ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1:
            createSimpleNotice();
            break;
        case 2:
            createDetailedNotice();
            break;
        case 3:
            return;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void PublicRelationsAdminInterface::createSimpleNotice() {
    clearScreen();
    displayHeader("CREATE SIMPLE NOTICE");

    string title, description;

    cout << "Enter notice title: ";
    getline(cin, title);

    cout << "Enter notice description: ";
    getline(cin, description);

    if (title.empty() || description.empty()) {
        displayError("Title and description cannot be empty!");
        pauseForInput();
        return;
    }

    if (currentPRAdmin->publishNotice(title, description)) {
        displaySuccess("Notice published successfully!");
    } else {
        displayError("Failed to publish notice!");
    }

    pauseForInput();
}

void PublicRelationsAdminInterface::createDetailedNotice() {
    clearScreen();
    displayHeader("CREATE DETAILED NOTICE");

    string title, description, audience;
    NoticeType type;

    cout << "Enter notice title: ";
    getline(cin, title);

    cout << "Enter notice description: ";
    getline(cin, description);

    cout << "Enter target audience (or press Enter for 'All'): ";
    getline(cin, audience);
    if (audience.empty()) audience = "All";

    type = selectNoticeType();

    if (title.empty() || description.empty()) {
        displayError("Title and description cannot be empty!");
        pauseForInput();
        return;
    }

    Notice notice = currentPRAdmin->createNotice(title, description, type);
    notice.setTargetAudience(audience);

    if (currentPRAdmin->publishNotice(notice)) {
        displaySuccess("Detailed notice published successfully!");
    } else {
        displayError("Failed to publish notice!");
    }

    pauseForInput();
}

NoticeType PublicRelationsAdminInterface::selectNoticeType() {
    clearScreen();
    displayHeader("SELECT NOTICE TYPE");

    cout << "1. General" << endl;
    cout << "2. Academic" << endl;
    cout << "3. Event" << endl;
    cout << "4. Emergency" << endl;
    cout << "5. Public Relations" << endl;
    cout << "6. Exam" << endl;
    cout << "7. Admission" << endl;
    cout << "8. Scholarship" << endl;

    int choice;
    cout << "Select notice type (1-8): ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1: return NoticeType::GENERAL;
        case 2: return NoticeType::ACADEMIC;
        case 3: return NoticeType::EVENT;
        case 4: return NoticeType::EMERGENCY;
        case 5: return NoticeType::PUBLIC_RELATIONS;
        case 6: return NoticeType::EXAM;
        case 7: return NoticeType::ADMISSION;
        case 8: return NoticeType::SCHOLARSHIP;
        default:
            displayInfo("Invalid choice! Defaulting to General.");
            return NoticeType::GENERAL;
    }
}

void PublicRelationsAdminInterface::handleNoticeManagement() {
    clearScreen();
    displayHeader("NOTICE MANAGEMENT");

    cout << "1. View All My Notices" << endl;
    cout << "2. Edit Notice" << endl;
    cout << "3. Delete Notice" << endl;
    cout << "4. Back to Main Menu" << endl;

    int choice;
    cout << "Select option (1-4): ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1:
            viewMyNotices();
            break;
        case 2:
            editNotice();
            break;
        case 3:
            deleteNotice();
            break;
        case 4:
            return;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void PublicRelationsAdminInterface::viewMyNotices() {
    clearScreen();
    displayHeader("MY PUBLISHED NOTICES");

    auto notices = currentPRAdmin->getMyPublishedNotices();

    if (notices.empty()) {
        displayInfo("No notices found.");
        pauseForInput();
        return;
    }

    displayNoticesList(notices);
    pauseForInput();
}

void PublicRelationsAdminInterface::displayNoticesList(const vector<Notice>& notices) {
    cout << left << setw(5) << "ID"
         << setw(30) << "Title"
         << setw(15) << "Type"
         << setw(12) << "Created"
         << setw(8) << "Status" << endl;
    displaySeparator('-', 70);

    for (const auto& notice : notices) {
        cout << left << setw(5) << notice.getNoticeID()
             << setw(30) << (notice.getTitle().length() > 28 ?
                            notice.getTitle().substr(0, 25) + "..." : notice.getTitle())
             << setw(15) << notice.getNoticeTypeString()
             << setw(12) << notice.getCreatedDate().toString()
             << setw(8) << (notice.getIsActive() ? "Active" : "Inactive") << endl;
    }
}

void PublicRelationsAdminInterface::handleNoticeSearch() {
    clearScreen();
    displayHeader("SEARCH & BROWSE NOTICES");

    cout << "1. Search by Keyword" << endl;
    cout << "2. Filter by Type" << endl;
    cout << "3. View All System Notices" << endl;
    cout << "4. Back to Main Menu" << endl;

    int choice;
    cout << "Select option (1-4): ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1:
            searchNotices();
            break;
        case 2:
            filterNoticesByType();
            break;
        case 3:
            viewAllNotices();
            break;
        case 4:
            return;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void PublicRelationsAdminInterface::searchNotices() {
    clearScreen();
    displayHeader("SEARCH NOTICES");

    string keyword;
    cout << "Enter search keyword: ";
    getline(cin, keyword);

    if (keyword.empty()) {
        displayError("Search keyword cannot be empty!");
        pauseForInput();
        return;
    }

    auto results = currentPRAdmin->searchNotices(keyword);

    if (results.empty()) {
        displayInfo("No notices found matching '" + keyword + "'.");
    } else {
        cout << "Found " << results.size() << " notice(s) matching '" << keyword << "':" << endl;
        displaySeparator('-', 50);
        displayNoticesList(results);
    }

    pauseForInput();
}

void PublicRelationsAdminInterface::handleNoticeStatistics() {
    clearScreen();
    currentPRAdmin->displayNoticeStatistics();
    pauseForInput();
}

void PublicRelationsAdminInterface::handleBulkOperations() {
    clearScreen();
    displayHeader("BULK OPERATIONS");

    cout << "1. Archive Old Notices" << endl;
    cout << "2. Back to Main Menu" << endl;

    int choice;
    cout << "Select option (1-2): ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1:
            archiveOldNotices();
            break;
        case 2:
            return;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void PublicRelationsAdminInterface::archiveOldNotices() {
    clearScreen();
    displayHeader("ARCHIVE OLD NOTICES");

    cout << "Enter cutoff date (DD-MM-YYYY) - notices before this date will be archived: ";
    string dateStr;
    getline(cin, dateStr);

    Date cutoffDate(dateStr);
    if (!cutoffDate.isValid()) {
        displayError("Invalid date format!");
        pauseForInput();
        return;
    }

    if (currentPRAdmin->archiveOldNotices(cutoffDate)) {
        displaySuccess("Old notices archived successfully!");
    } else {
        displayInfo("No notices found to archive.");
    }

    pauseForInput();
}

void PublicRelationsAdminInterface::handleProfile() {
    clearScreen();
    displayHeader("PROFILE SETTINGS");

    cout << "Current Profile Information:" << endl;
    currentPRAdmin->display();

    cout << "\nProfile management features:" << endl;
    cout << "• Update personal information" << endl;
    cout << "• Change password" << endl;
    cout << "• View account statistics" << endl;

    pauseForInput();
}

void PublicRelationsAdminInterface::handleLogout() {
    displayHeader("LOGOUT");
    cout << "Are you sure you want to logout? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        displaySuccess("Logged out successfully!");
        isRunning = false;
    }
}

void PublicRelationsAdminInterface::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}

// Additional helper methods for completeness
void PublicRelationsAdminInterface::viewAllNotices() {
    clearScreen();
    displayHeader("ALL SYSTEM NOTICES");

    auto notices = currentPRAdmin->getAllNotices();

    if (notices.empty()) {
        displayInfo("No notices found in the system.");
        pauseForInput();
        return;
    }

    cout << "Total notices in system: " << notices.size() << endl;
    displaySeparator('-', 50);
    displayNoticesList(notices);
    pauseForInput();
}

void PublicRelationsAdminInterface::editNotice() {
    clearScreen();
    displayHeader("EDIT NOTICE");

    auto myNotices = currentPRAdmin->getMyPublishedNotices();
    if (myNotices.empty()) {
        displayInfo("You have no notices to edit.");
        pauseForInput();
        return;
    }

    cout << "Your notices:" << endl;
    displayNoticesList(myNotices);

    size_t noticeID;
    cout << "Enter notice ID to edit: ";
    cin >> noticeID;
    cin.ignore();

    if (!currentPRAdmin->isMyNotice(noticeID)) {
        displayError("Notice not found or you don't have permission to edit it.");
        pauseForInput();
        return;
    }

    // Find the notice
    Notice* noticeToEdit = nullptr;
    for (auto& notice : myNotices) {
        if (notice.getNoticeID() == noticeID) {
            noticeToEdit = &notice;
            break;
        }
    }

    if (!noticeToEdit) {
        displayError("Notice not found.");
        pauseForInput();
        return;
    }

    string newTitle, newDescription;
    cout << "Current title: " << noticeToEdit->getTitle() << endl;
    cout << "Enter new title (or press Enter to keep current): ";
    getline(cin, newTitle);

    cout << "Current description: " << noticeToEdit->getDescription() << endl;
    cout << "Enter new description (or press Enter to keep current): ";
    getline(cin, newDescription);

    if (!newTitle.empty()) noticeToEdit->setTitle(newTitle);
    if (!newDescription.empty()) noticeToEdit->setDescription(newDescription);

    if (currentPRAdmin->updateNotice(noticeID, *noticeToEdit)) {
        displaySuccess("Notice updated successfully!");
    } else {
        displayError("Failed to update notice!");
    }

    pauseForInput();
}

void PublicRelationsAdminInterface::deleteNotice() {
    clearScreen();
    displayHeader("DELETE NOTICE");

    auto myNotices = currentPRAdmin->getMyPublishedNotices();
    if (myNotices.empty()) {
        displayInfo("You have no notices to delete.");
        pauseForInput();
        return;
    }

    cout << "Your notices:" << endl;
    displayNoticesList(myNotices);

    size_t noticeID;
    cout << "Enter notice ID to delete: ";
    cin >> noticeID;
    cin.ignore();

    if (!currentPRAdmin->isMyNotice(noticeID)) {
        displayError("Notice not found or you don't have permission to delete it.");
        pauseForInput();
        return;
    }

    cout << "Are you sure you want to delete this notice? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        if (currentPRAdmin->withdrawNotice(noticeID)) {
            displaySuccess("Notice deleted successfully!");
        } else {
            displayError("Failed to delete notice!");
        }
    } else {
        displayInfo("Delete operation cancelled.");
    }

    pauseForInput();
}

void PublicRelationsAdminInterface::filterNoticesByType() {
    clearScreen();
    displayHeader("FILTER NOTICES BY TYPE");

    NoticeType type = selectNoticeType();
    displayNoticesByType(type);
}

void PublicRelationsAdminInterface::displayNoticesByType(NoticeType type) {
    clearScreen();
    displayHeader("NOTICES: " + NoticeTypeHelper::toString(type));

    auto notices = currentPRAdmin->getNoticesByType(type);

    if (notices.empty()) {
        displayInfo("No notices found for type: " + NoticeTypeHelper::toString(type));
    } else {
        cout << "Found " << notices.size() << " notice(s) of type " << NoticeTypeHelper::toString(type) << ":" << endl;
        displaySeparator('-', 50);
        displayNoticesList(notices);
    }

    pauseForInput();
}
