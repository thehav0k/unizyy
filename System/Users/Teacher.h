//
// Created by Md. Asif Khan on 8/8/25.
//

#ifndef TEACHER_H
#define TEACHER_H
#include "user.h"
#include <string>
#include <vector>
#include <fstream>
#include "student.h"

#include "../../Core/Models/Course.h"
#include "../../Core/Models/Designation.h"
#include "../../Core/Models/department.h"
#include "../../Core/Models/Notice.h"
#include "../../Core/Database/DatabaseManager.h"

class Teacher: public User {
private:
    char name[100];
    department dept;
    designation AP;

public:
    // Constructors
    Teacher(const string &email, const string &password, const string &name, const string &department, designation rank);
    Teacher();

    // Getters and setters
    void setName(const string &name);
    string getName() const;
    void setDepartment(department dept);
    department getDepartment() const;
    string getDepartmentName() const;
    void setRank(designation AP);
    designation getDesignation() const;

    // Virtual methods
    void display() const override;
    void gradeStudent(Student&, Course&);

    // Notice management methods
    Notice createNotice(const string& title, const string& description, NoticeType type) const;
    Notice createDetailedNotice(const string& title, const string& description, NoticeType type,
                               const string& targetAudience = "All") const;
    bool publishNotice(const Notice& notice) const;
    vector<Notice> getMyPublishedNotices() const;
    bool updateNotice(size_t noticeID, const Notice& updatedNotice) const;
    bool withdrawNotice(size_t noticeID) const;

    // Static database operations
    static vector<Teacher> loadAllTeachers();
    static bool addTeacherToDB(const Teacher& teacher);
    static bool updateTeacherInDB(const string& email, const Teacher& updatedTeacher);
    static bool deleteTeacherFromDB(const string& email);
    static Teacher* findTeacherByEmail(const string& email);
};

#endif //TEACHER_H
