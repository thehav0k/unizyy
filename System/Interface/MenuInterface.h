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

    // eita sobar upore thakbe && ekhane current time display hbe
    // Date class theke current Time call korlei hbe
    // title ta print hobe sathe kichu design korlei hbe
    void displayHeader(const string& title);

    // lomba Line print = diya
    void displaySeparator(char ch = '=', int length = 50);
    // clear screen
    void clearScreen();
    // input newar jnno Basic thama
    // just enter input niye continue kora
    void pauseForInput();
    // yes no confirm
    bool confirmAction(const string& message);
    // message gula display kora basic cout diye
    void displayError(const string& message);
    void displaySuccess(const string& message);
    void displayInfo(const string& message);
};

#endif //MENU_INTERFACE_H