//
// Created by Md. Asif Khan on 10/8/25.
//

#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>
#include <iostream>
using namespace std;

enum class department {
    Department_of_Computer_Science_and_Engineering,
    Department_of_physics,
    Department_of_Mathematics,
    // Add more departments as needed
};
//header file execute kora jay na, eikhane inline use kora hoise jaate code gula
//executable function e directly copy paste hoy
static inline const char* toString(department dept) {
    switch (dept) {
        case department::Department_of_Computer_Science_and_Engineering: return "Computer Science and Engineering";
        case department::Department_of_physics: return "Physics";
        case department::Department_of_Mathematics: return "Mathematics";
        default: return "Unknown Department";
    }
}
//eita nao lagte pare

#endif //DEPARTMENT_H
