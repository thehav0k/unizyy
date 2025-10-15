#include "TeacherInterface.h"
#include "../Authentication/auth.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include "../../Core/Models/date.h"

using namespace std;

//Constructor
TeacherInterface::TeacherInterface(Teacher* teacher, Auth* auth)
    : currentTeacher(teacher), authSystem(auth), isRunning(true) {}

//Shows dashboard
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
    cout << "1. View Profile" << endl;
    cout << "2. Logout" << endl;
    displaySeparator('-', 40);
}

//User theke input ney
int TeacherInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (0-2): ";

    while (!(cin >> choice) || choice < 0 || choice > 2) {
        displayError("Invalid input! Please enter a number between 0-2.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (0-2): ";
    }

    cin.ignore();
    return choice;
}

//Choice onujayi operation kaj kore
void TeacherInterface::processChoice(int choice) {
    switch (choice) {
        case 0: {
            displayHeader("SIMULATE DATE");
            cout << "Enter date (DD-MM-YYYY) or leave empty to reset: ";
            string input;
            getline(cin, input);
            if (input.empty()) {
                Date::resetSimulation();
                displaySuccess("Simulation reset to current date.");
            } else {
                Date d(input);
                if (!d.isValid()) {
                    displayError("Invalid date format.");
                } else {
                    Date::setSimulatedDate(d);
                    displaySuccess("Simulation date set to " + d.toString());
                }
            }
            pauseForInput();
            break;
        }
        case 1:
            handleProfile();
            break;
        case 2:
            handleLogout();
            break;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
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

//Main run loop — jotokkhon isRunning true thakbe cholte thakbe
void TeacherInterface::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}
