//
// Created by Md. Asif Khan on 11/8/25.
// Teacher Interface Implementation
//

#include "TeacherInterface.h"
#include "../Authentication/auth.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include "../../Core/Models/date.h"

using namespace std;

TeacherInterface::TeacherInterface(Teacher* teacher, Auth* auth)
    : currentTeacher(teacher), authSystem(auth), isRunning(true) {}

void TeacherInterface::displayTeacherDashboard() {
    clearScreen();
    displayHeader("TEACHER DASHBOARD");
    cout << "Welcome, " << currentTeacher->getName() << "!" << endl;
    cout << "Department: " << currentTeacher->getDepartmentName() << endl;
    cout << "Designation: " << toString(currentTeacher->getDesignation()) << endl;
    cout << "Email: " << currentTeacher->getEmail() << endl;
    displaySeparator('-', 60);
}

void TeacherInterface::displayMenu() {
    displayTeacherDashboard();

    cout << "0. Simulate Date" << endl;
    cout << "\nTEACHER MENU OPTIONS:" << endl;
    cout << "1. Course Management" << endl;
    cout << "2. Student Grading" << endl;
    cout << "3. Attendance Management" << endl;
    cout << "4. Notice Management" << endl;
    cout << "5. Profile Settings" << endl;
    cout << "6. Logout" << endl;
    displaySeparator('-', 40);
}

int TeacherInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (0-6): ";

    while (!(cin >> choice) || choice < 0 || choice > 6) {
        displayError("Invalid input! Please enter a number between 0-6.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (0-6): ";
    }

    cin.ignore();
    return choice;
}

void TeacherInterface::processChoice(int choice) {
    switch (choice) {
        case 0: {
            displayHeader("SIMULATE DATE");
            cout << "Enter date (DD-MM-YYYY) or leave empty to reset: ";
            string input;
            getline(cin, input);
            if (input.empty()) {
                Date::resetSimulation(); displaySuccess("Simulation reset to current date.");
            } else {
                Date d(input);
                if (!d.isValid()) displayError("Invalid date format.");
                else { Date::setSimulatedDate(d); displaySuccess("Simulation date set to " + d.toString()); }
            }
            pauseForInput();
        } break;
        case 1:
            handleCourseManagement();
            break;
        case 2:
            handleStudentGrading();
            break;
        case 3:
            handleAttendance();
            break;
        case 4:
            handleNoticeManagement();
            break;
        case 5:
            handleProfile();
            break;
        case 6:
            handleLogout();
            break;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void TeacherInterface::handleCourseManagement() {
    displayHeader("COURSE MANAGEMENT");
    displayInfo("Course management features:");
    cout << "• Create and manage course content" << endl;
    cout << "• Set course schedules and assignments" << endl;
    cout << "• View enrolled students" << endl;
    cout << "• Manage course materials" << endl;
    pauseForInput();
}

void TeacherInterface::handleStudentGrading() {
    displayHeader("STUDENT GRADING");
    displayInfo("Grading system features:");
    cout << "• Grade student assignments and exams" << endl;
    cout << "• View student performance analytics" << endl;
    cout << "• Generate grade reports" << endl;
    cout << "• Manage assessment criteria" << endl;
    pauseForInput();
}

void TeacherInterface::handleAttendance() {
    displayHeader("ATTENDANCE MANAGEMENT");
    displayInfo("Attendance tracking features:");
    cout << "• Record daily attendance" << endl;
    cout << "• View attendance reports" << endl;
    cout << "• Generate attendance analytics" << endl;
    cout << "• Send notifications for low attendance" << endl;
    pauseForInput();
}

void TeacherInterface::handleProfile() {
    displayHeader("TEACHER PROFILE");
    currentTeacher->display();
    pauseForInput();
}

void TeacherInterface::handleLogout() {
    if (confirmAction("Are you sure you want to logout?")) {
        displayInfo("Logged out successfully!");
        isRunning = false;
    }
}

void TeacherInterface::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}

void TeacherInterface::handleNoticeManagement() {
    clearScreen();
    displayHeader("NOTICE MANAGEMENT");

    cout << "1. Create Notice" << endl;
    cout << "2. View My Notices" << endl;
    cout << "3. View All Notices" << endl;
    cout << "4. Search Notices" << endl;
    cout << "5. Edit My Notice" << endl;
    cout << "6. Delete My Notice" << endl;
    cout << "7. Back to Main Menu" << endl;

    int choice;
    cout << "Select option (1-7): ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1:
            createNotice();
            break;
        case 2:
            viewMyNotices();
            break;
        case 3:
            viewAllNotices();
            break;
        case 4:
            searchNotices();
            break;
        case 5:
            editMyNotice();
            break;
        case 6:
            deleteMyNotice();
            break;
        case 7:
            return;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void TeacherInterface::createNotice() {
    clearScreen();
    displayHeader("CREATE NOTICE");

    string title, description, audience;

    cout << "Enter notice title: ";
    getline(cin, title);

    cout << "Enter notice description: ";
    getline(cin, description);

    cout << "Enter target audience (or press Enter for 'All'): ";
    getline(cin, audience);
    if (audience.empty()) audience = "All";

    NoticeType type = selectNoticeType();

    if (title.empty() || description.empty()) {
        displayError("Title and description cannot be empty!");
        pauseForInput();
        return;
    }

    Notice notice = currentTeacher->createDetailedNotice(title, description, type, audience);

    if (currentTeacher->publishNotice(notice)) {
        displaySuccess("Notice published successfully!");
    } else {
        displayError("Failed to publish notice!");
    }

    pauseForInput();
}

NoticeType TeacherInterface::selectNoticeType() {
    clearScreen();
    displayHeader("SELECT NOTICE TYPE");

    cout << "1. General" << endl;
    cout << "2. Academic" << endl;
    cout << "3. Event" << endl;
    cout << "4. Department" << endl;
    cout << "5. Exam" << endl;

    int choice;
    cout << "Select notice type (1-5): ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1: return NoticeType::GENERAL;
        case 2: return NoticeType::ACADEMIC;
        case 3: return NoticeType::EVENT;
        case 4: return NoticeType::DEPARTMENT;
        case 5: return NoticeType::EXAM;
        default:
            displayInfo("Invalid choice! Defaulting to Academic.");
            return NoticeType::ACADEMIC;
    }
}

void TeacherInterface::viewMyNotices() {
    clearScreen();
    displayHeader("MY PUBLISHED NOTICES");

    auto notices = currentTeacher->getMyPublishedNotices();

    if (notices.empty()) {
        displayInfo("No notices found.");
        pauseForInput();
        return;
    }

    displayNoticesList(notices);
    pauseForInput();
}

void TeacherInterface::viewAllNotices() {
    clearScreen();
    displayHeader("ALL SYSTEM NOTICES");

    auto notices = Notice::loadFromFile("notices");

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

void TeacherInterface::searchNotices() {
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

    auto allNotices = Notice::loadFromFile("notices");
    vector<Notice> results;

    for (const auto& notice : allNotices) {
        if (notice.getTitle().find(keyword) != string::npos ||
            notice.getDescription().find(keyword) != string::npos) {
            results.push_back(notice);
        }
    }

    if (results.empty()) {
        displayInfo("No notices found matching '" + keyword + "'.");
    } else {
        cout << "Found " << results.size() << " notice(s) matching '" << keyword << "':" << endl;
        displaySeparator('-', 50);
        displayNoticesList(results);
    }

    pauseForInput();
}

void TeacherInterface::displayNoticesList(const vector<Notice>& notices) {
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

void TeacherInterface::editMyNotice() {
    clearScreen();
    displayHeader("EDIT MY NOTICE");

    auto myNotices = currentTeacher->getMyPublishedNotices();
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

    // Find the notice
    Notice* noticeToEdit = nullptr;
    for (auto& notice : myNotices) {
        if (notice.getNoticeID() == noticeID) {
            noticeToEdit = &notice;
            break;
        }
    }

    if (!noticeToEdit) {
        displayError("Notice not found or you don't have permission to edit it.");
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

    if (currentTeacher->updateNotice(noticeID, *noticeToEdit)) {
        displaySuccess("Notice updated successfully!");
    } else {
        displayError("Failed to update notice!");
    }

    pauseForInput();
}

void TeacherInterface::deleteMyNotice() {
    clearScreen();
    displayHeader("DELETE MY NOTICE");

    auto myNotices = currentTeacher->getMyPublishedNotices();
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

    // Verify ownership
    bool isOwner = false;
    for (const auto& notice : myNotices) {
        if (notice.getNoticeID() == noticeID) {
            isOwner = true;
            break;
        }
    }

    if (!isOwner) {
        displayError("Notice not found or you don't have permission to delete it.");
        pauseForInput();
        return;
    }

    cout << "Are you sure you want to delete this notice? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        if (currentTeacher->withdrawNotice(noticeID)) {
            displaySuccess("Notice deleted successfully!");
        } else {
            displayError("Failed to delete notice!");
        }
    } else {
        displayInfo("Delete operation cancelled.");
    }

    pauseForInput();
}
