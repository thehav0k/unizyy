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
