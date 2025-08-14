//
// Created by Md. Asif Khan on 11/8/25.
// Base Menu Interface
//

#ifndef MENU_INTERFACE_H
#define MENU_INTERFACE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class MenuInterface {
public:
    virtual ~MenuInterface() = default;
    virtual void displayMenu() = 0;
    virtual int getChoice() = 0;
    virtual void processChoice(int choice) = 0;
    virtual void run() = 0;

protected:
    void displayHeader(const string& title);
    void displaySeparator(char ch = '=', int length = 50);
    void clearScreen();
    void pauseForInput();
    bool confirmAction(const string& message);
    void displayError(const string& message);
    void displaySuccess(const string& message);
    void displayInfo(const string& message);

    // Utility for realtime display
    string getCurrentTimeString() const;
};

#endif //MENU_INTERFACE_H
