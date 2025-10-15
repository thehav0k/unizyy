#include "PublicRelationsAdminInterface.h"
#include "../Authentication/auth.h"
#include "../../Core/Models/date.h"
#include "../../Core/Database/DatabaseManager.h"
#include <iostream>
#include <limits>

using namespace std;

//Constructor
PublicRelationsAdminInterface::PublicRelationsAdminInterface(PublicRelationsAdmin* admin, Auth* auth)
    : currentAdmin(admin), authSystem(auth), isRunning(true) {
}
//Showing dashboard of admin
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
    cout << "3. Delete Notice" << endl;
    cout << "4. View Notice Details" << endl;
    cout << "5. Notice Statistics" << endl;
    cout << "6. View Profile" << endl;
    cout << "7. Change Password" << endl;
    cout << "8. Logout" << endl;
    displaySeparator('-', 40);
}

//User er choice ney
int PublicRelationsAdminInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (1-8): ";

    while (!(cin >> choice) || choice < 1 || choice > 8) {
        displayError("Invalid input! Please enter a number between 1-8.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (1-8): ";
    }

    cin.ignore();
    return choice;
}
//User er choice onujaye operation chalay
void PublicRelationsAdminInterface::processChoice(int choice) {
    switch (choice) {
        case 1:
            handleCreateNotice();
            break;
        case 2:
            handleViewAllNotices();
            break;
        case 3:
            handleDeleteNotice();
            break;
        case 4:
            handleViewNoticeDetails();
            break;
        case 5:
            handleNoticeStatistics();
            break;
        case 6:
            handleViewProfile();
            break;
        case 7:
            handleChangePassword();
            break;
        case 8:
            handleLogout();
            break;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}
//Main interface run kore and loop e cholte thake
void PublicRelationsAdminInterface::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}

//Create Notice
void PublicRelationsAdminInterface::handleCreateNotice() {
    clearScreen();
    displayHeader("CREATE NEW NOTICE");
    currentAdmin->createNotice();
    pauseForInput();
}

//Sob notice dekha
void PublicRelationsAdminInterface::handleViewAllNotices() {
    clearScreen();
    displayHeader("ALL NOTICES");
    currentAdmin->viewAllNotices();
    pauseForInput();
}
//Delete Notice
void PublicRelationsAdminInterface::handleDeleteNotice() {
    clearScreen();
    displayHeader("DELETE NOTICE");
    currentAdmin->deleteNotice();
    pauseForInput();
}

//Nirdishto Notice dekha
void PublicRelationsAdminInterface::handleViewNoticeDetails() {
    clearScreen();
    displayHeader("NOTICE DETAILS");
    currentAdmin->viewNoticeDetails();
    pauseForInput();
}

void PublicRelationsAdminInterface::handleNoticeStatistics() {
    clearScreen();
    displayHeader("NOTICE STATISTICS");
    currentAdmin->displayNoticeStatistics();
    pauseForInput();
}

//Admin er profile dekhabe
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

//Logout korar handle kore
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
