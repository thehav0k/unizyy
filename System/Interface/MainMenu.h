//
// Created by Md. Asif Khan on 11/8/25.
//

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "MenuInterface.h"

// Agei class declare // Jate ei class e use kora jay
class Auth;

class MainMenu : public MenuInterface {
private:
    Auth* authSystem;
    bool isRunning; // while loop e use korar jnno

public:
    // explicit use korchi jate hudai auth to mainmenu object convert na hoy
    explicit MainMenu(Auth* auth);

    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override;
    void run() override;

private:
    void displayWelcomeBanner();
    // login email & pass collect
    // user class er pointer diye dynamic binding kore different user interface call korbo
    // then boom

    void handleLogin();
    // login er motoi
    // just usertype agei input niye appropriate details niye id khula
    // ekhaneo dynamic binding hoiche and casting o
    // input validation er jnno auth class er methods use kora jete pare
    void handleRegistration();
    // About section er lekha gula display
    void handleAbout();

    // exit the program
    // isRunning false hbe
    void handleExit();
};
#endif //MAIN_MENU_H
