//
// Created by Md. Asif Khan on 8/8/25.
//
#include "student.h"
#include <iostream>
#include <iomanip> // Include iomanip for setprecision
#include "../../Core/Database/DatabaseManager.h"

using namespace std;

void Student::display() const {
    cout << "Student Details:" << endl;
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Student ID: " << studentID << endl;
    cout << "Class Roll: " << classRoll << endl;
    cout << "Department: " << toString(dept) << endl;
    cout << "Batch: " << batch << endl;
    cout << "Hall: " << hallToString(hall) << endl;
    cout << "Gender: " << getGenderString() << endl;
    cout << "Balance: ৳" << fixed << setprecision(2) << balance << " BDT" << endl;
}