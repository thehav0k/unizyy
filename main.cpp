#include <iostream>
//Ei program ta multi-file program,karon onek file include kore ekshathe kaj korche(6)
#include "System/Authentication/auth.h"
#include "System/Interface/MainMenu.h"

using namespace std;

int main() {
    try {
        Auth authSystem; //Auth class er ekta object authSystem create hoyeche(3)
        MainMenu mainMenu(&authSystem);
        mainMenu.run();

    } catch (const exception& e) {
        cout << "System Error: " << e.what() << endl;
        cout << "Please contact system administrator." << endl;
        return 1;
    }

    return 0;
}
