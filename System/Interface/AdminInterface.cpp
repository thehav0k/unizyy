//
// Created by Md. Asif Khan on 11/8/25.
// Admin Interface Implementation
//

#include "AdminInterface.h"
#include "../Authentication/auth.h"
#include <iostream>
#include <limits>
#include "../../Core/Models/date.h"

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
    cout << "4. Database Management" << endl;
    cout << "5. Profile Settings" << endl;
    cout << "6. Logout" << endl;
    displaySeparator('-', 40);
}

int AdminInterface::getChoice() {
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
            handleDatabaseManagement();
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

void AdminInterface::handleUserManagement() {
    displayHeader("USER MANAGEMENT");
    displayInfo("User management features will be implemented here:");
    cout << "• Add new users (Students, Teachers, Dining Authorities)" << endl;
    cout << "• View user details and statistics" << endl;
    cout << "• Modify user permissions" << endl;
    cout << "• Deactivate/Activate user accounts" << endl;
    pauseForInput();
}

void AdminInterface::handleSystemSettings() {
    displayHeader("SYSTEM SETTINGS");
    displayInfo("System settings features:");
    cout << "• Configure meal time windows" << endl;
    cout << "• Set system-wide policies" << endl;
    cout << "• Manage hall configurations" << endl;
    cout << "• Update system parameters" << endl;
    pauseForInput();
}

void AdminInterface::handleReports() {
    displayHeader("SYSTEM REPORTS");
    displayInfo("Comprehensive system reporting:");
    cout << "• User activity reports" << endl;
    cout << "• Meal token usage statistics" << endl;
    cout << "• Revenue analytics across all halls" << endl;
    cout << "• System performance metrics" << endl;
    pauseForInput();
}

void AdminInterface::handleDatabaseManagement() {
    displayHeader("DATABASE MANAGEMENT");
    displayInfo("Database management tools:");
    cout << "• Backup and restore operations" << endl;
    cout << "• Data cleanup and maintenance" << endl;
    cout << "• System integrity checks" << endl;
    cout << "• Performance optimization" << endl;
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
