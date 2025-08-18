//
// Created by Md. Asif Khan on 8/8/25.
//

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

void Teacher::gradeStudent(Student &student, Course &course) {
    cout << "Grading student: " << student.getName() << " for course " << course.getCourseName() << endl;
}

vector<Teacher> Teacher::loadAllTeachers() {
    return DatabaseManager::loadTeachers();
}

bool Teacher::addTeacherToDB(const Teacher& teacher) {
    return DatabaseManager::addTeacher(teacher);
}

bool Teacher::updateTeacherInDB(const string& email, const Teacher& updatedTeacher) {
    return DatabaseManager::updateTeacher(email, updatedTeacher);
}

bool Teacher::deleteTeacherFromDB(const string& email) {
    return DatabaseManager::deleteTeacher(email);
}

Teacher* Teacher::findTeacherByEmail(const string& email) {
    return DatabaseManager::findTeacherByEmail(email);
}
