//
// Created by Md. Asif Khan on 10/8/25.
//

#pragma once

#include <string>
#include <vector>
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

    static User* login(const string& email, const string& password);

    static bool registerStudent(const string &studentID, const string &name, const string &email,
                        int age, int classRoll, department dept, int batch,
                        Halls hall, const string &password);
    static bool registerTeacher(const string &name, const string &email, const string &department,
                        designation rank, const string &password);
    static bool registerAdmin(const string &name, const string &email, AdminType adminType, const string &password);
    static bool registerDiningAuthority(const string &name, const string &email, const string &hallName,
                                const string &designation, const string &phoneNumber, const string &password);

    static bool isEmailRegistered(const string& email);
    static DiningAuthority* getDiningAuthorityByEmail(const string& email);
    static void displayAllUsers();
    void logout();

    static string getValidatedEmail();
    static string getValidatedPassword();
    static string getValidatedStudentID();
    static string getValidatedName();
};

// Specialized admin registration function
bool registerAdminByAdmin(const string &currentAdminEmail, const string &name, const string &email, AdminType adminType, const string &password);

// Utility functions
void initializeDatabase();
void loadDataFromFiles();
void saveDataToFiles();
bool isAnyAdminRegistered();
Student* getStudentByEmail(const string& email);
Teacher* getTeacherByEmail(const string& email);
Admin* getAdminByEmail(const string& email);
void clearAuthData();
