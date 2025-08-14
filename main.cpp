//
// Created by Md. Asif Khan on 11/8/25.
// University Management System Main Entry Point
//

#include <iostream>
#include "System/Authentication/auth.h"
#include "System/Interface/MainMenu.h"

using namespace std;

int main() {
    try {
        // Initialize authentication system
        Auth authSystem;

        // Create and run main menu interface
        MainMenu mainMenu(&authSystem);
        mainMenu.run();

    } catch (const exception& e) {
        cout << "System Error: " << e.what() << endl;
        cout << "Please contact system administrator." << endl;
        return 1;
    }

    return 0;
}
