//
// Created by Md. Asif Khan on 11/8/25.
// Enhanced Menu Interface Implementation with Modern UI
//

#include "MenuInterface.h"
#include "../../Core/Models/date.h"
#include <iostream>
#include <iomanip>
#include <thread>

using namespace std;

void MenuInterface::displayHeader(const string& title) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  " << title << " - " << Date::getCurrentTimeString() << endl;
    cout << string(60, '=') << endl;
}

void MenuInterface::displaySeparator(char ch, int length) {
    cout << string(length, ch) << endl;
}

void MenuInterface::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void MenuInterface::pauseForInput() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

bool MenuInterface::confirmAction(const string& message) {
    char choice;
    cout << "[WARNING]  " << message << " (y/n): ";
    cin >> choice;
    cin.ignore();
    return (choice == 'y' || choice == 'Y');
}

void MenuInterface::displayError(const string& message) {
    cout << "\nERROR: " << message << endl;
}

void MenuInterface::displaySuccess(const string& message) {
    cout << "\nSUCCESS: " << message << endl;
}

void MenuInterface::displayInfo(const string& message) {
    cout << "\nINFO: " << message << endl;
}

string MenuInterface::getCurrentTimeString() const {
    return Date::getCurrentTimeString();
}
