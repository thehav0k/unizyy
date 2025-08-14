//
// Created by Md. Asif Khan on 11/8/25.
// Enhanced Menu Interface Implementation with Modern UI
//

#include "MenuInterface.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>

using namespace std;

void MenuInterface::displayHeader(const string& title) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  " << title << " - " << getCurrentTimeString() << endl;
    cout << string(60, '=') << endl;
}

void MenuInterface::displaySeparator(char ch, int length) {
    cout << string(length, ch) << endl;
}

void MenuInterface::clearScreen() {
    // Cross-platform clear screen
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void MenuInterface::pauseForInput() {
    cout << "\n📌 Press Enter to continue...";
    cin.get();
}

bool MenuInterface::confirmAction(const string& message) {
    char choice;
    cout << "⚠️  " << message << " (y/n): ";
    cin >> choice;
    cin.ignore();
    return (choice == 'y' || choice == 'Y');
}

void MenuInterface::displayError(const string& message) {
    cout << "\n❌ ERROR: " << message << endl;
}

void MenuInterface::displaySuccess(const string& message) {
    cout << "\n✅ SUCCESS: " << message << endl;
}

void MenuInterface::displayInfo(const string& message) {
    cout << "\n💡 INFO: " << message << endl;
}

string MenuInterface::getCurrentTimeString() const {
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    auto tm = *localtime(&time_t);

    stringstream ss;
    ss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
