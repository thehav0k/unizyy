//
// Created by Md. Asif Khan on 11/8/25.
// Main Menu Interface for the University System
//

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "MenuInterface.h"

// Forward declaration
class Auth;

class MainMenu : public MenuInterface {
private:
    Auth* authSystem;
    bool isRunning;

public:
    explicit MainMenu(Auth* auth);

    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override;
    void run() override;

private:
    void displayWelcomeBanner();
    void handleLogin();
    void handleRegistration();
    void handleAbout();
    void handleExit();
};

#endif //MAIN_MENU_H
