//
// Created by Md. Asif Khan on 11/8/25.
// Admin Interface Implementation
//

#include "AdminInterface.h"
#include "../Authentication/auth.h"
#include <iostream>
#include <limits>
#include "../../Core/Models/date.h"
#include<NoticeManager.h>

using namespace std;

AdminInterface::AdminInterface(Admin* admin, Auth* auth)
    : currentAdmin(admin), authSystem(auth), isRunning(true) {}

void AdminInterface::displayAdminDashboard() {
    clearScreen();
    displayHeader("ADMIN DASHBOARD");
    cout << "Welcome, " << currentAdmin->getName() << "!" << endl;
    cout << "Admin Type: " << toString(currentAdmin->getAdminType()) << endl;
    cout << "Email: " << currentAdmin->getEmail() << endl;
    displaySeparator('-', 60);
}

void AdminInterface::displayMenu() {
    displayAdminDashboard();

    cout << "0. Simulate Date" << endl;
    cout << "\nADMIN MENU OPTIONS:" << endl;
    cout << "1. User Management" << endl;
    cout << "2. System Settings" << endl;
    cout << "3. System Reports" << endl;
    cout << "4. Notice Management(create,update,view)" << endl;
    cout << "5. Database Management" << endl;
    cout << "6. Profile Settings" << endl;
    cout << "7. Logout" << endl;
    displaySeparator('-', 40);
}

int AdminInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (0-7): ";

    while (!(cin >> choice) || choice < 0 || choice > 7) {
        displayError("Invalid input! Please enter a number between 0-6.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (0-7): ";
    }

    cin.ignore();
    return choice;
}

void AdminInterface::processChoice(int choice) {
    switch (choice) {
        case 0: {
            displayHeader("SIMULATE DATE");
            cout << "Enter date (DD-MM-YYYY) or leave empty to reset: ";
            string input;
            getline(cin, input);
            if (input.empty()) { Date::resetSimulation(); displaySuccess("Simulation reset to current date."); }
            else { Date d(input); if (!d.isValid()) displayError("Invalid date format."); else { Date::setSimulatedDate(d); displaySuccess("Simulation date set to " + d.toString()); }}
            pauseForInput();
        } break;
        case 1:
            handleUserManagement();
            break;
        case 2:
            handleSystemSettings();
            break;
        case 3:
            handleReports();
            break;
        case 4:
            handleNoticeManagement();
            break;
        case 5:
            handleDatabaseManagement();
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

void AdminInterface::handleUserManagement() {
    displayHeader("USER MANAGEMENT");
    displayInfo("User management features will be implemented here:");
    cout << "-> Add new users (Students, Teachers, Dining Authorities)" << endl;
    cout << "-> View user details and statistics" << endl;
    cout << "-> Modify user permissions" << endl;
    cout << "-> Deactivate/Activate user accounts" << endl;
    pauseForInput();
}

void AdminInterface::handleSystemSettings() {
    displayHeader("SYSTEM SETTINGS");
    displayInfo("System settings features:");
    cout << "-> Configure meal time windows" << endl;
    cout << "-> Set system-wide policies" << endl;
    cout << "-> Manage hall configurations" << endl;
    cout << "-> Update system parameters" << endl;
    pauseForInput();
}

void AdminInterface::handleReports() {
    displayHeader("SYSTEM REPORTS");
    displayInfo("Comprehensive system reporting:");
    cout << "-> User activity reports" << endl;
    cout << "-> Meal token usage statistics" << endl;
    cout << "-> Revenue analytics across all halls" << endl;
    cout << "-> System performance metrics" << endl;
    pauseForInput();
}

void AdminInterface::handleNoticeManagement() {
    clearScreen();
    displayHeader("NOTICE MANAGEMENT");

    static vector<Notice> notices;

    while (true) {
        cout << "1. Create Notice" << endl;
        cout << "2. Update Notice" << endl;
        cout << "3. View All Notices" << endl;
        cout << "4. Back to Admin Menu" << endl;
        cout << "Enter choice from 1 to 4: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string title, message,author;
            cout << "Enter notice title: ";
            getline(cin, title);
            cout << "Enter message: ";
            getline(cin, message);
            author = currentAdmin->getName();
            Notice n(title, message, Date::getCurrentDate());
            notices.push_back(n);
            displaySuccess("You've created the notice successfully!!!");

        } else if (choice == 2) {
            if (notices.empty()) {
                displayInfo("Nothing   to update.");
            } else {
                for (int i = 0; i < notices.size(); i++) {
                    cout << i << ". " << notices[i].getTitle() << endl;
                }
                cout << "Enter notice index to update: ";
                int indx;
                cin >> indx;
                cin.ignore();
                if (indx< 0 || indx >= (int)notices.size()) {
                    displayError("Invalid index!");
                } else {
                    string nTitle, nMessage;
                    cout << "Enter new title: ";
                    getline(cin, nTitle);
                    cout << "Enter new message: ";
                    getline(cin, nMessage);
                    notices[indx].setTitle(nTitle);
                    notices[indx].setMessage(nMessage);
                    displaySuccess("Notice updated successfully!");
                }
            }

        } else if (choice == 3) {
            if (notices.empty()) {
                displayInfo("Nothing to show.");
            } else {
                for (const auto& n : notices) {
                    cout << "\nTitle: " << n.getTitle() << endl;
                    cout << "Date: " << n.getDate().toString() << endl;
                    cout << "Message: " << n.getMessage() << endl;
                    displaySeparator('-', 50);
                }
            }


        } else if (choice == 4) {
            break;
        } else {
            displayError("The  option is not valid!");
        }

        pauseForInput();
        clearScreen();
        displayHeader("NOTICE MANAGEMENT");
    }
}

void AdminInterface::handleDatabaseManagement() {
    displayHeader("DATABASE MANAGEMENT");
    displayInfo("Database management tools:");
    cout << "-> Backup and restore operations" << endl;
    cout << "-> Data cleanup and maintenance" << endl;
    cout << "-> System integrity checks" << endl;
    cout << "-> Performance optimization" << endl;
    pauseForInput();
}

void AdminInterface::handleProfile() {
    displayHeader("ADMIN PROFILE");
    currentAdmin->display();
    pauseForInput();
}

void AdminInterface::handleLogout() {
    if (confirmAction("Are you sure you want to logout?")) {
        displayInfo("Logged out successfully!");
        isRunning = false;
    }
}

void AdminInterface::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}
