//
// Created by Md. Asif Khan on 11/8/25.
//

#include "MainMenu.h"
#include "../Authentication/auth.h"
#include "StudentInterface.h"
#include "AdminInterface.h"
#include "TeacherInterface.h"
#include "DiningAuthorityInterface.h"
#include <iostream>
#include <limits>
#include "../../Core/Models/date.h"  // for date simulation

using namespace std;

MainMenu::MainMenu(Auth* auth) : authSystem(auth), isRunning(true) {}

void MainMenu::displayWelcomeBanner() {
    clearScreen();
    cout << "\nUNIZY : JAHANGIRNAGAR UNIVERSITY MANAGEMENT SYSTEM" << endl;
    cout << "==========================================================" << endl;
    cout<< Date::getCurrentDateTimeString() << endl;
    cout << "==========================================================" << endl;
}

void MainMenu::displayMenu() {
    displayWelcomeBanner();
    cout << "\nMAIN MENU:" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "0. Simulate Date & Time" << endl;
    cout << "1. Login to System" << endl;
    cout << "2. Register New Account" << endl;
    cout << "3. About System & Features" << endl;
    cout << "4. Exit Application" << endl;
    cout << "----------------------------------------------------------" << endl;
}
