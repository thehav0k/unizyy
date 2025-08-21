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
    cout << "4. Profile Settings" << endl;
    cout << "5. Logout" << endl;
    displaySeparator('-', 40);
}

int TeacherInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (0-5): ";

    while (!(cin >> choice) || choice < 0 || choice > 5) {
        displayError("Invalid input! Please enter a number between 0-5.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (0-5): ";
    }

    cin.ignore();
    return choice;
}
