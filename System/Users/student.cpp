//
// Created by Md. Asif Khan on 8/8/25.
//
#include "student.h"
#include <iostream>
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
}

// Static database operations
vector<Student> Student::loadAllStudents() {
    return DatabaseManager::loadStudents();
}
bool Student::addStudentToDB(const Student& student) {
    return DatabaseManager::addStudent(student);
}
bool Student::updateStudentInDB(const string& studentID, const Student& updatedStudent) {
    return DatabaseManager::updateStudent(studentID, updatedStudent);
}
bool Student::deleteStudentFromDB(const string& studentID) {
    return DatabaseManager::deleteStudent(studentID);
}
Student* Student::findStudentByEmail(const string& email) {
    return DatabaseManager::findStudentByEmail(email);
}
Student* Student::findStudentByID(const string& studentID) {
    return DatabaseManager::findStudentByID(studentID);
}
