//
// Created by Md. Asif Khan on 10/8/25.
//

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "../Users/user.h"
#include "../Users/student.h"
#include "../../Core/Models/hall.h"
#include "../Users/Teacher.h"
#include "../Users/Admin.h"
#include "../Users/DiningAuthority.h"
#include "../../Core/Models/Designation.h"
#include "../../Core/Models/admintype.h"
#include "../../Core/Database/DatabaseManager.h"

using namespace std;

// Auth class for handling authentication operations
class Auth {
private:
    DatabaseManager dbManager;

public:
    Auth();

    // Login method that returns a User pointer
    static User* login(const string& email, const string& password);

    // Registration methods
    static bool registerStudent(const string &studentID, const string &name, const string &email,
                        int age, int classRoll, department dept, int batch,
                        Halls hall, const string &password);

    static bool registerTeacher(const string &name, const string &email, const string &department,
                        AcademicPosition rank, const string &password);

    static bool registerAdmin(const string &name, const string &email, AdminType adminType, const string &password);

    static bool registerDiningAuthority(const string &name, const string &email, const string &hallName,
                                const string &designation, const string &phoneNumber, const string &password);

    // Utility methods
    static bool isEmailRegistered(const string& email);
    static DiningAuthority* getDiningAuthorityByEmail(const string& email);
    static void displayAllUsers();
    void logout();

private:
    // Helper methods for user authentication
    Student* authenticateStudent(const string& email, const string& password);
    Teacher* authenticateTeacher(const string& email, const string& password);
    Admin* authenticateAdmin(const string& email, const string& password);
    DiningAuthority* authenticateDiningAuthority(const string& email, const string& password);
};

// Legacy function declarations (for backward compatibility)
bool registerStudentLegacy(const string &studentID, const string &name, const string &email,
                    int age, int classRoll, department dept, int batch,
                    Halls hall, const string &password);

bool registerTeacherLegacy(const string &name, const string &email, const string &department,
                    AcademicPosition rank, const string &password);

bool registerAdminLegacy(const string &name, const string &email, AdminType adminType, const string &password);

bool registerAdminByAdmin(const string &name, const string &email, AdminType adminType, const string &password);
