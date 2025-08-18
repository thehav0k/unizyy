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
            handleProfile();
            break;
        case 5:
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
