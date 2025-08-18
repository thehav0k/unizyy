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

// Notice management methods
// Notice class improve kora lagbe
Notice Teacher::createNotice(const string& title, const string& description, NoticeType type) const {
    Notice notice(title, description, type, getName(), getEmail());
    notice.setTargetDepartment(dept);
    return notice;
}

Notice Teacher::createDetailedNotice(const string& title, const string& description, NoticeType type,
                                   const string& targetAudience) const {
    Notice notice(title, description, type, getName(), getEmail());
    notice.setTargetDepartment(dept);
    notice.setTargetAudience(targetAudience);
    return notice;
}

bool Teacher::publishNotice(const Notice& notice) const {
    // Use the Notice class's saveToFile method which uses DatabaseManager
    return notice.saveToFile("notices");
}

vector<Notice> Teacher::getMyPublishedNotices() const {
    // Get all notices and filter by author email
    vector<Notice> allNotices = Notice::loadFromFile("notices");

    vector<Notice> myNotices;
    for (const auto& notice : allNotices) {
        if (notice.getAuthorEmail() == getEmail()) {
            myNotices.push_back(notice);
        }
    }
    return myNotices;
}

bool Teacher::updateNotice(size_t noticeID, const Notice& updatedNotice) const {
    // Load all notices
    vector<Notice> allNotices = Notice::loadFromFile("notices");

    // Find and update the notice if it belongs to this teacher
    bool found = false;
    for (auto& notice : allNotices) {
        if (notice.getNoticeID() == noticeID && notice.getAuthorEmail() == getEmail()) {
            notice = updatedNotice;
            found = true;
            break;
        }
    }

    if (found) {
        // Save all notices back to file
        DatabaseManager::saveObjects(allNotices, "notices");
        return true;
    }
    return false;
}

bool Teacher::withdrawNotice(size_t noticeID) const {
    vector<Notice> allNotices = Notice::loadFromFile("notices");

    auto it = allNotices.begin();
    while (it != allNotices.end()) {
        if (it->getNoticeID() == noticeID && it->getAuthorEmail() == getEmail()) {
            it = allNotices.erase(it);
            DatabaseManager::saveObjects(allNotices, "notices");
            return true;
        } else {
            it++;
        }
    }
    return false;
}
