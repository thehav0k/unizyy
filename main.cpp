#include <iostream>
#include "System/Authentication/auth.h"
#include "System/Interface/MainMenu.h"

using namespace std;

int main() {
    try {
        Auth authSystem;
        MainMenu mainMenu(&authSystem);
        mainMenu.run();

    } catch (const exception& e) {
        cout << "System Error: " << e.what() << endl;
        cout << "Please contact system administrator." << endl;
        return 1;
    }

    return 0;
}
