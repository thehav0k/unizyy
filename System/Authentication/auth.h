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

class Auth {
private:
    DatabaseManager dbManager;// karon authentication basically login+reg and duitai basically database operation
public:
    Auth();
    // login er jnno database(basically cached vector) theke searching kora lagbe
    static User* login(const string& email, const string& password);
    // cache vector e new object push_back kora lagbe (student teacher admin sob same)
    static bool registerStudent(const string &studentID, const string &name, const string &email,int age, int classRoll, department dept, int batch,
                        Halls hall, const string &password);
    static bool registerTeacher(const string &name, const string &email, const department &dept,designation desg, const string &password);
    static bool registerAdmin(const string &name, const string &email, AdminType adminType, const string &password);
    static bool registerDiningAuthority(const string &name, const string &email, const string &hallName,const string &password);
   // search by using vector.find()
    // thakle reg korte dewa jabena
    static bool isEmailRegistered(const string& email);
    static DiningAuthority* getDiningAuthorityByEmail(const string& email);
    static void displayAllUsers();
    void logout();

    static string getValidatedEmail();
    static string getValidatedPassword();
    static string getValidatedStudentID();
    static string getValidatedName();

};

// only system admin onno admin der registration korte parbe
bool registerAdminByAdmin(const string &currentAdminEmail, const string &name, const string &email, AdminType adminType, const string &password);

//Helper functions
void initializeDatabase();
void loadDataFromFiles();
void saveDataToFiles();
bool isAnyAdminRegistered();
Student* getStudentByEmail(const string& email);
Teacher* getTeacherByEmail(const string& email);
Admin* getAdminByEmail(const string& email);
void clearAuthData();
