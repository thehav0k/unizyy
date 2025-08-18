//
// Created by Md. Asif Khan on 10/8/25.
//

#include "auth.h"
#include "../Users/student.h"
#include "../Users/Teacher.h"
#include "../Users/Admin.h"
#include "../Users/DiningAuthority.h"
#include "../../Core/Database/DatabaseManager.h"
#include "../../Core/Utils/StringHelper.h"
#include <iostream>

using namespace std;
// ei issue resolve kora lagbe
void initializeDatabase() {
    DatabaseManager::initializeDatabase();
}

void loadDataFromFiles() {
    DatabaseManager::initializeDatabase();
}

void saveDataToFiles() {
    cout << "Data synchronized with database." << endl;
}

bool registerAdminByAdmin(const string& currentAdminEmail, const string& name, const string& email, AdminType adminType, const string& password) {
    // system admin kina check
    Admin* currentAdmin = DatabaseManager::findAdminByEmail(currentAdminEmail);
    if (!currentAdmin || currentAdmin->getAdminType() != AdminType::SystemAdmin) {
        cout << "Error: Only System Admins can register new admins." << endl;
        return false;
    }
    if (adminType == AdminType::SystemAdmin) {
        if (DatabaseManager::getSystemAdminCount() >= 3) {
            cout << "Error: Maximum of 3 System Admins allowed. Current count: "
                 << DatabaseManager::getSystemAdminCount() << endl;
            return false;
        }
    }
    if (name.empty()) {
        cout << "Error: Name cannot be empty." << endl;
        return false;
    }
    if (!StringHelper::validateEmail(email)) {
        cout << "Error: Invalid email format." << endl;
        return false;
    }
    if (!StringHelper::validatePassword(password)) {
        cout << "Error: Password must be at least 6 characters long." << endl;
        return false;
    }
    if (DatabaseManager::emailExists(email)) {
        cout << "Error: Email already exists." << endl;
        return false;
    }
    Admin newAdmin(email, password, name, adminType);
    DatabaseManager::addAdmin(newAdmin);

    cout << "Admin registration by admin successful!" << endl;
    cout << "New " << toString(adminType) << " admin created." << endl;
    if (adminType == AdminType::SystemAdmin) {
        cout << "System Admin count: " << DatabaseManager::getSystemAdminCount() << "/3" << endl;
    }
    return true;
}

bool isAnyAdminRegistered() {
    return DatabaseManager::getAdminCount() > 0;
}

Student* getStudentByEmail(const string& email) {
    return DatabaseManager::findStudentByEmail(email);
}

Teacher* getTeacherByEmail(const string& email) {
    return DatabaseManager::findTeacherByEmail(email);
}

Admin* getAdminByEmail(const string& email) {
    return DatabaseManager::findAdminByEmail(email);
}

void clearAuthData() {
    DatabaseManager::clearAllData();
    cout << "All authentication data cleared." << endl;
}

// Auth class implementations
Auth::Auth() {
    DatabaseManager::initializeDatabase();
}

User* Auth::login(const string& email, const string& password) {
    if (!StringHelper::validateEmail(email)) {
        cout << "Error: Invalid email format." << endl;
        return nullptr;
    }

    Student* student = DatabaseManager::findStudentByEmail(email);
    if (student && student->getPassword() == password) {
        return student;
    }

    Teacher* teacher = DatabaseManager::findTeacherByEmail(email);
    if (teacher && teacher->getPassword() == password) {
        return teacher;
    }

    Admin* admin = DatabaseManager::findAdminByEmail(email);
    if (admin && admin->getPassword() == password) {
        return admin;
    }

    DiningAuthority* diningAuth = DatabaseManager::findDiningAuthorityByEmail(email);
    if (diningAuth && diningAuth->getPassword() == password) {
        return diningAuth;
    }
    cout << "Error: Invalid email or password." << endl;
    return nullptr;
}

void Auth::logout() {
}

bool Auth::registerStudent(const string &studentID, const string &name, const string &email,
                            int age, int classRoll, department dept, int batch,
                            Halls hall, const string &password) {
    if (name.empty() || studentID.empty()) {
        cout << "Error: Name and Student ID cannot be empty." << endl;
        return false;
    }

    if (!StringHelper::isValidstudentID(studentID)) {
        cout << "Error: Invalid Student ID format. Must be exactly 11 digits." << endl;
        return false;
    }

    if (!StringHelper::validateEmail(email)) {
        cout << "Error: Invalid email format." << endl;
        return false;
    }

    if (!StringHelper::validatePassword(password)) {
        cout << "Error: Password must be at least 6 characters long." << endl;
        return false;
    }

    if (DatabaseManager::emailExists(email)) {
        cout << "Error: Email already exists." << endl;
        return false;
    }

    if (DatabaseManager::studentIDExists(studentID)) {
        cout << "Error: Student ID already exists." << endl;
        return false;
    }

    if (age < 16 || age > 35) {
        cout << "Error: Age must be between 16 and 35." << endl;
        return false;
    }

    Student newStudent(email, password, name, age, studentID, classRoll, dept, batch, hall);
    DatabaseManager::addStudent(newStudent);

    cout << "Student registered successfully!" << endl;
    return true;
}

bool Auth::registerTeacher(const string &name, const string &email, const string &department,
                            designation desg, const string &password) {
    if (name.empty() || department.empty()) {
        cout << "Error: Name and Department cannot be empty." << endl;
        return false;
    }

    if (!StringHelper::validateEmail(email)) {
        cout << "Error: Invalid email format." << endl;
        return false;
    }

    if (!StringHelper::validatePassword(password)) {
        cout << "Error: Password must be at least 6 characters long." << endl;
        return false;
    }

    if (DatabaseManager::emailExists(email)) {
        cout << "Error: Email already exists." << endl;
        return false;
    }

    Teacher newTeacher(email, password, name, department, desg);
    DatabaseManager::addTeacher(newTeacher);

    cout << "Teacher registered successfully!" << endl;
    return true;
}

bool Auth::registerAdmin(const string &name, const string &email, AdminType adminType, const string &password) {
    // jodi admin na thake tahle admin reg korte parbe
    if (DatabaseManager::getAdminCount() > 0) {
        cout << "Error: Admin already exists. Use registerAdminByAdmin for additional admins." << endl;
        return false;
    }

    if (name.empty()) {
        cout << "Error: Name cannot be empty." << endl;
        return false;
    }

    if (!StringHelper::validateEmail(email)) {
        cout << "Error: Invalid email format." << endl;
        return false;
    }

    if (!StringHelper::validatePassword(password)) {
        cout << "Error: Password must be at least 6 characters long." << endl;
        return false;
    }

    if (DatabaseManager::emailExists(email)) {
        cout << "Error: Email already exists." << endl;
        return false;
    }

    Admin newAdmin(email, password, name, adminType);
    DatabaseManager::addAdmin(newAdmin);

    cout << "Admin registered successfully!" << endl;
    return true;
}

bool Auth::registerDiningAuthority(const string &name, const string &email, const string &hallName,
                                   const string &designation, const string &phoneNumber, const string &password) {
    if (name.empty() || hallName.empty()) {
        cout << "Error: Name and Hall cannot be empty." << endl;
        return false;
    }
    if (!StringHelper::validateEmail(email) || !StringHelper::validatePassword(password)) {
        cout << "Error: Invalid email or password format." << endl;
        return false;
    }

    if (DatabaseManager::emailExists(email)) {
        cout << "Error: Email already exists." << endl;
        return false;
    }

    DiningAuthority newAuth(email, password, name, hallName);
    DatabaseManager::addDiningAuthority(newAuth);
    cout << "Dining authority registered successfully!" << endl;
    return true;
}

bool Auth::isEmailRegistered(const string &email) {
    return DatabaseManager::emailExists(email);
}

DiningAuthority* Auth::getDiningAuthorityByEmail(const string& email) {
    return DatabaseManager::findDiningAuthorityByEmail(email);
}

void Auth::displayAllUsers() {
    cout << "\n=== ALL REGISTERED USERS ===" << endl;

    vector<Student> students = DatabaseManager::loadStudents();
    vector<Teacher> teachers = DatabaseManager::loadTeachers();
    vector<Admin> admins = DatabaseManager::loadAdmins();
    vector<DiningAuthority> diningAuthorities = DatabaseManager::loadDiningAuthorities();

    cout << "\n--- STUDENTS ---" << endl;
    if (students.empty()) {
        cout << "No students registered." << endl;
    } else {
        for (size_t i = 0; i < students.size(); ++i) {
            cout << "Student " << (i + 1) << ":" << endl;
            students[i].display();
            cout << "---" << endl;
        }
    }

    cout << "\n--- TEACHERS ---" << endl;
    if (teachers.empty()) {
        cout << "No teachers registered." << endl;
    } else {
        for (size_t i = 0; i < teachers.size(); ++i) {
            cout << "Teacher " << (i + 1) << ":" << endl;
            teachers[i].display();
            cout << "---" << endl;
        }
    }

    cout << "\n--- ADMINS ---" << endl;
    if (admins.empty()) {
        cout << "No admins registered." << endl;
    } else {
        for (size_t i = 0; i < admins.size(); ++i) {
            cout << "Admin " << (i + 1) << ":" << endl;
            admins[i].display();
            cout << "---" << endl;
        }
    }

    cout << "\n--- DINING AUTHORITIES ---" << endl;
    if (diningAuthorities.empty()) {
        cout << "No dining authorities registered." << endl;
    } else {
        for (size_t i = 0; i < diningAuthorities.size(); ++i) {
            cout << "Dining Authority " << (i + 1) << ":" << endl;
            diningAuthorities[i].display();
            cout << "---" << endl;
        }
    }

    DatabaseManager::displayDatabaseStats();
}

// Static validation helper methods
string Auth::getValidatedEmail() {
    string email;
    bool isValid = false;

    while (!isValid) {
        cout << "Enter Email (@juniv.edu): ";
        getline(cin, email);

        if (email.empty()) {
            cout << "Email cannot be empty. Please try again." << endl;
            continue;
        }

        if (!StringHelper::validateEmail(email)) {
            cout << "Invalid email format. Requirements:" << endl;
            cout << "   • Must end with @juniv.edu" << endl;
            cout << "   • Must have at least 4 characters before @" << endl;
            cout << "   • Example: john1234@juniv.edu" << endl;
            continue;
        }

        if (DatabaseManager::emailExists(email)) {
            cout << "This email is already registered. Please use a different email." << endl;
            continue;
        }

        isValid = true;
    }

    return email;
}

string Auth::getValidatedPassword() {
    string password;
    bool isValid = false;

    while (!isValid) {
        cout << "Enter Password: ";
        getline(cin, password);

        if (password.empty()) {
            cout << "Password cannot be empty. Please try again." << endl;
            continue;
        }

        if (!StringHelper::validatePassword(password)) {
            cout << "Password does not meet requirements:" << endl;
            cout << "   • At least 6 characters long" << endl;
            cout << "   • Must contain at least one uppercase letter" << endl;
            cout << "   • Must contain at least one lowercase letter" << endl;
            cout << "   • Must contain at least one digit" << endl;
            cout << "   • Example: MyPass123" << endl;
            continue;
        }

        isValid = true;
    }

    return password;
}

string Auth::getValidatedStudentID() {
    string studentID;
    bool isValid = false;

    while (!isValid) {
        cout << "Enter Student ID: ";
        getline(cin, studentID);

        if (studentID.empty()) {
            cout << "Student ID cannot be empty. Please try again." << endl;
            continue;
        }

        if (!StringHelper::isValidstudentID(studentID)) {
            cout << "Invalid Student ID format. Requirements:" << endl;
            cout << "   • Must be exactly 11 digits" << endl;
            cout << "   • Example: 12345678901" << endl;
            continue;
        }

        if (DatabaseManager::studentIDExists(studentID)) {
            cout << "This Student ID is already registered. Please use a different ID." << endl;
            continue;
        }

        isValid = true;
    }

    return studentID;
}

string Auth::getValidatedName() {
    string name;
    bool isValid = false;

    while (!isValid) {
        cout << "Enter Name: ";
        getline(cin, name);

        if (!StringHelper::isValidString(name)) {
            cout << "Invalid name. Requirements:" << endl;
            cout << "   • Cannot be empty" << endl;
            cout << "   • Cannot start with a space" << endl;
            cout << "   • Must contain at least one non-space character" << endl;
            continue;
        }

        isValid = true;
    }

    return name;
}
