#include "Teacher.h"
#include <iostream>
#include "../../Core/Models/department.h"
#include "../../Core/Utils/StringHelper.h"
#include "../../Core/Database/DatabaseManager.h"

using namespace std;

Teacher::Teacher(const string &email, const string &password, const string &name,
                 const string &department, designation desg)
        : User(email, password), AP(desg) {
    StringHelper::stringToCharArray(name, this->name);
    this->dept = stringToDepartmentEnum(department);
}

Teacher::Teacher() : User(), dept(department::Department_of_Computer_Science_and_Engineering),
                     AP(designation::Lecturer) {
    name[0] = '\0';
}

// Getters and setters
void Teacher::setName(const string &name) {
    StringHelper::stringToCharArray(name, this->name);
}

string Teacher::getName() const {
    return StringHelper::charArrayToString(name);
}

void Teacher::setDepartment(department dept) {
    this->dept = dept;
}

department Teacher::getDepartment() const {
    return dept;
}

string Teacher::getDepartmentName() const {
    return toString(dept);
}

void Teacher::setRank(designation AP) {
    this->AP = AP;
}
designation Teacher::getDesignation() const {
    return AP;
}

// Display All teachers
void Teacher::display() const {
    cout << "Teacher Details:" << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Department: " << toString(dept) << endl;
    cout << "Rank: " << toString(AP) << endl;
}