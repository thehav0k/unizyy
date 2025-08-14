//
// Created by Md. Asif Khan on 8/8/25.
//

#ifndef COURSE_H
#define COURSE_H
#include <string>
#include <iostream>
#include "../Utils/StringHelper.h"
using namespace std;

class Course {
private:
    char courseName[60];
    char courseCode[20];
    double totalMarks;
public:
    Course(const string& courseName, const string& courseCode, double totalMarks)
        : totalMarks(totalMarks) {
        StringHelper::stringToCharArray(courseName, this->courseName);
        StringHelper::stringToCharArray(courseCode, this->courseCode);
    }
    // Default constructor
    Course() : totalMarks(0.0) {
        courseName[0] = '\0';
        courseCode[0] = '\0';
    }

    string getCourseName() const {
        return StringHelper::charArrayToString(courseName);
    }

    void setCourseName(const string& courseName) {
        StringHelper::stringToCharArray(courseName, this->courseName);
    }

    string getCourseCode() const {
        return StringHelper::charArrayToString(courseCode);
    }

    void setCourseCode(const string& courseCode) {
        StringHelper::stringToCharArray(courseCode, this->courseCode);
    }

    double getTotalMarks() const {
        return totalMarks;
    }

    void setTotalMarks(double totalMarks) {
        this->totalMarks = totalMarks;
    }

    void marksToCGPA();

    void display() const {
        cout << "Course Name: " << courseName << endl;
        cout << "Course Code: " << courseCode << endl;
        cout << "Total Marks: " << totalMarks << endl;
    }

};
#endif
