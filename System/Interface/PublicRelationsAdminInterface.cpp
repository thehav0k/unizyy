//
// Created by Md. Asif Khan on 12/10/25.
//

#include "PublicRelationsAdminInterface.h"
#include "../Authentication/auth.h"
#include "../../Core/Models/date.h"
#include "../../Core/Database/DatabaseManager.h"
#include <iostream>
#include <limits>

using namespace std;

PublicRelationsAdminInterface::PublicRelationsAdminInterface(PublicRelationsAdmin* admin, Auth* auth)
    : currentAdmin(admin), authSystem(auth), isRunning(true) {
}

void PublicRelationsAdminInterface::displayAdminDashboard() {
    clearScreen();
    displayHeader("PUBLIC RELATIONS ADMIN DASHBOARD");
    cout << "Welcome, " << currentAdmin->getName() << "!" << endl;
    cout << "Email: " << currentAdmin->getEmail() << endl;
    cout << "Role: " << toString(currentAdmin->getAdminType()) << endl;
    displaySeparator('-', 60);
}

void PublicRelationsAdminInterface::displayMenu() {
    displayAdminDashboard();
    cout << "\nNOTICE MANAGEMENT MENU:" << endl;
    cout << "1. Create New Notice" << endl;
    cout << "2. View All Notices" << endl;
    cout << "3. Edit Notice" << endl;
    cout << "4. Delete Notice" << endl;
    cout << "5. View Notice Details" << endl;
    cout << "6. Search Notices" << endl;
    cout << "7. Notice Statistics" << endl;
    cout << "8. View Profile" << endl;
    cout << "9. Change Password" << endl;
    cout << "10. Logout" << endl;
    displaySeparator('-', 40);
}

int PublicRelationsAdminInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (1-10): ";

    while (!(cin >> choice) || choice < 1 || choice > 10) {
        displayError("Invalid input! Please enter a number between 1-10.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (1-10): ";
    }

    cin.ignore();
    return choice;
}

void PublicRelationsAdminInterface::processChoice(int choice) {
    switch (choice) {
        case 1:
            handleCreateNotice();
            break;
        case 2:
            handleViewAllNotices();
            break;
        case 3:
            handleEditNotice();
            break;
        case 4:
            handleDeleteNotice();
            break;
        case 5:
            handleViewNoticeDetails();
            break;
        case 6:
            handleSearchNotices();
            break;
        case 7:
            handleNoticeStatistics();
            break;
        case 8:
            handleViewProfile();
            break;
        case 9:
            handleChangePassword();
            break;
        case 10:
            handleLogout();
            break;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void PublicRelationsAdminInterface::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}

void PublicRelationsAdminInterface::handleCreateNotice() {
    clearScreen();
    displayHeader("CREATE NEW NOTICE");
    currentAdmin->createNotice();
    pauseForInput();
}

void PublicRelationsAdminInterface::handleViewAllNotices() {
    clearScreen();
    displayHeader("ALL NOTICES");
    currentAdmin->viewAllNotices();
    pauseForInput();
}

void PublicRelationsAdminInterface::handleEditNotice() {
    clearScreen();
    displayHeader("EDIT NOTICE");
    currentAdmin->editNotice();
    pauseForInput();
}

void PublicRelationsAdminInterface::handleDeleteNotice() {
    clearScreen();
    displayHeader("DELETE NOTICE");
    currentAdmin->deleteNotice();
    pauseForInput();
}

void PublicRelationsAdminInterface::handleViewNoticeDetails() {
    clearScreen();
    displayHeader("NOTICE DETAILS");
    currentAdmin->viewNoticeDetails();
    pauseForInput();
}

void PublicRelationsAdminInterface::handleSearchNotices() {
    clearScreen();
    displayHeader("SEARCH NOTICES");
    currentAdmin->searchNotices();
    pauseForInput();
}

void PublicRelationsAdminInterface::handleNoticeStatistics() {
    clearScreen();
    displayHeader("NOTICE STATISTICS");
    currentAdmin->displayNoticeStatistics();
    pauseForInput();
}

void PublicRelationsAdminInterface::handleViewProfile() {
    clearScreen();
    displayHeader("MY PROFILE");
    currentAdmin->display();
    pauseForInput();
}

void PublicRelationsAdminInterface::handleChangePassword() {
    clearScreen();
    displayHeader("CHANGE PASSWORD");

    string currentPassword, newPassword, confirmPassword;

    cout << "Enter current password: ";
    getline(cin, currentPassword);

    cout << "Enter new password: ";
    getline(cin, newPassword);

    cout << "Confirm new password: ";
    getline(cin, confirmPassword);

    if (newPassword != confirmPassword) {
        displayError("Passwords do not match!");
        pauseForInput();
        return;
    }

    if (currentAdmin->changePassword(currentPassword, newPassword)) {
        displaySuccess("Password changed successfully!");

        // Update in database
        if (DatabaseManager::updateAdmin(currentAdmin->getEmail(), *currentAdmin)) {
            displayInfo("Changes saved to database.");
        } else {
            displayError("Failed to save changes to database!");
        }
    } else {
        displayError("Failed to change password. Current password may be incorrect.");
    }

    pauseForInput();
}

void PublicRelationsAdminInterface::handleLogout() {
    clearScreen();
    displayHeader("LOGOUT");

    cout << "Are you sure you want to logout? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        displaySuccess("Logging out...");
        isRunning = false;
    } else {
        displayInfo("Logout cancelled.");
        pauseForInput();
    }
}

