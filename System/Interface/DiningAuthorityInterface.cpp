//
// Created by Md. Asif Khan on 11/8/25.
// Dining Authority Interface Implementation
//

#include "DiningAuthorityInterface.h"
#include "../Authentication/auth.h"
#include <iostream>
#include <limits>

using namespace std;

DiningAuthorityInterface::DiningAuthorityInterface(DiningAuthority* authority, Auth* auth)
    : currentAuthority(authority), authSystem(auth), isRunning(true) {}

void DiningAuthorityInterface::displayAuthorityDashboard() {
    clearScreen();
    displayHeader("DINING AUTHORITY DASHBOARD");
    cout << "Welcome, " << currentAuthority->getName() << "!" << endl;
    cout << "Managing Hall: " << currentAuthority->getHallName() << endl;
    cout << "Email: " << currentAuthority->getEmail() << endl;
    displaySeparator('-', 60);
}

void DiningAuthorityInterface::displayMenu() {
    displayAuthorityDashboard();
    cout << "\nDINING AUTHORITY MENU:" << endl;
    cout << "1. Meal Management" << endl;
    cout << "2. Reports & Analytics" << endl;
    cout << "3. Profile Settings" << endl;
    cout << "4. Logout" << endl;
    displaySeparator('-', 40);
}

