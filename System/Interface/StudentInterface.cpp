
#include "StudentInterface.h"
#include "../Authentication/auth.h"
#include "../../Core/Models/date.h"
#include "../../Core/Database/DatabaseManager.h"
#include "../../Core/Utils/StringHelper.h"
#include <iostream>
#include <limits>
#include <iomanip>

using namespace std;

StudentInterface::StudentInterface(Student* student, Auth* auth)
    : currentStudent(student), authSystem(auth), isRunning(true) {
    tokenManager = new TokenManager();
}

void StudentInterface::displayStudentDashboard() {
    clearScreen();
    displayHeader("STUDENT DASHBOARD");
    cout << "Welcome, " << currentStudent->getName() << "!" << endl;
    cout << "Student ID: " << currentStudent->getStudentID() << endl;
    cout << "Hall: " << hallToString(currentStudent->getHall()) << endl;
    displaySeparator('-', 60);
}

void StudentInterface::displayMenu() {
    displayStudentDashboard();
    cout << "\nSTUDENT MENU OPTIONS:" << endl;
    cout << "0. Simulate Date" << endl;
    cout << "1. Meal Token Operations" << endl;
    cout << "2. View Profile" << endl;
    cout << "3. Change Password" << endl;
    cout << "4. Logout" << endl;
    displaySeparator('-', 40);
}

int StudentInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (0-4): ";

    while (!(cin >> choice) || choice < 0 || choice > 4) {
        displayError("Invalid input! Please enter a number between 0-4.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (0-4): ";
    }

    cin.ignore();
    return choice;
}
