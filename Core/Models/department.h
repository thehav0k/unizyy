#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>
#include <iostream>

enum class department {
    Department_of_Computer_Science_and_Engineering,
    Department_of_physics,
    Department_of_Mathematics,
    // Add more departments as needed
};
//header file execute kora jay na, eikhane inline use kora hoise jaate code gula
//executable function e directly copy paste hoy
inline const char* toString(department dept) {
    switch (dept) {
        case department::Department_of_Computer_Science_and_Engineering: return "Computer Science and Engineering";
        case department::Department_of_physics: return "Physics";
        case department::Department_of_Mathematics: return "Mathematics";
        // Add more cases as needed
        default: return "Unknown Department";
    }
}

#endif //DEPARTMENT_H
