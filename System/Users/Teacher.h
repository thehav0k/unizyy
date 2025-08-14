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
#include "../../Core/Database/DatabaseManager.h"

class Teacher: public User {
private:
    char name[100];        // Changed to char array
    department dept;       // Fixed: use dept consistently
    AcademicPosition AP;

public:
    // Constructors
    Teacher(const string &email, const string &password, const string &name, const string &department, AcademicPosition rank);
    Teacher();

    // Getters and setters
    void setName(const string &name);
    string getName() const;
    void setDepartment(department dept);
    department getDepartment() const;
    string getDepartmentName() const;  // New method for interface compatibility
    void setRank(AcademicPosition AP);
    AcademicPosition getRank() const;
    AcademicPosition getDesignation() const;  // Alias for getRank() for interface compatibility

    // Virtual methods
    void display() const override;

    // Additional methods
    // Grading student
    void gradeStudent(Student&, Course&);

    // Static database operations
    static vector<Teacher> loadAllTeachers();
    static bool addTeacherToDB(const Teacher& teacher);
    static bool updateTeacherInDB(const string& email, const Teacher& updatedTeacher);
    static bool deleteTeacherFromDB(const string& email);
    static Teacher* findTeacherByEmail(const string& email);
};

#endif //TEACHER_H
