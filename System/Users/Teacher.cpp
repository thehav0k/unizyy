//
// Created by Md. Asif Khan on 8/8/25.
//

#include "Teacher.h"
#include <iostream>
#include "../../Core/Models/department.h"
#include "../../Core/Utils/StringHelper.h"
#include "../../Core/Database/DatabaseManager.h"

using namespace std;

// Constructors
Teacher::Teacher(const string &email, const string &password, const string &name,
                 const string &department, AcademicPosition rank)
    : User(email, password), AP(rank) {
    // Use StringHelper for conversion
    StringHelper::stringToCharArray(name, this->name);
    // Convert string department to enum using the centralized function
    this->dept = stringToDepartmentEnum(department);
}

Teacher::Teacher() : User(), dept(department::Department_of_Computer_Science_and_Engineering),
                     AP(AcademicPosition::Lecturer) {
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

void Teacher::setRank(AcademicPosition AP) {
    this->AP = AP;
}

AcademicPosition Teacher::getRank() const {
    return AP;
}

AcademicPosition Teacher::getDesignation() const {
    return getRank();  // Alias method for interface compatibility
}

// Virtual methods
void Teacher::display() const {
    cout << "Teacher Details:" << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Department: " << toString(dept) << endl;
    cout << "Rank: " << toString(AP) << endl;
}

// Additional methods
void Teacher::gradeStudent(Student &student, Course &course) {
    // Implementation for grading student
    cout << "Grading student: " << student.getName() << " for course " << course.getCourseName() << endl;
}

// Static database operations
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
