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
#include <regex>

using namespace std;


void initializeDatabase() {
    DatabaseManager::initializeDatabase();
}

void loadDataFromFiles() {
    DatabaseManager::initializeDatabase();
}

void saveDataToFiles() {
    cout << "Data synchronized with database." << endl;
}

// Utility functions
bool isValidEmail(const string& email) {
    return StringHelper::validateEmail(email);
}

bool isValidPassword(const string& password) {
    return StringHelper::validatePassword(password);
}

// Student registration - simplified using DatabaseManager
bool registerStudent(const string& studentID, const string& name, const string& email,
                    int age, int classRoll, department dept, int batch,
                    Halls hall, const string& password) {

    // Validate input
    if (name.empty() || studentID.empty()) {
        cout << "Error: Name and Student ID cannot be empty." << endl;
        return false;
    }

    if (!isValidEmail(email)) {
        cout << "Error: Invalid email format." << endl;
        return false;
    }

    if (!isValidPassword(password)) {
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

    // Create and add new student - directly using Student class
    Student newStudent(email, password, name, age, studentID, classRoll, dept, batch, hall);
    DatabaseManager::addStudent(newStudent);

    cout << "Student registered successfully!" << endl;
    return true;
}

// Teacher registration - simplified using DatabaseManager
bool registerTeacher(const string& name, const string& email, const string& department,
                    AcademicPosition rank, const string& password) {

    // Validate input
    if (name.empty() || department.empty()) {
        cout << "Error: Name and Department cannot be empty." << endl;
        return false;
    }

    if (!isValidEmail(email)) {
        cout << "Error: Invalid email format." << endl;
        return false;
    }

    if (!isValidPassword(password)) {
        cout << "Error: Password must be at least 6 characters long." << endl;
        return false;
    }

    if (DatabaseManager::emailExists(email)) {
        cout << "Error: Email already exists." << endl;
        return false;
    }

    // Create and add new teacher
    Teacher newTeacher(email, password, name, department, rank);
    DatabaseManager::addTeacher(newTeacher);

    cout << "Teacher registered successfully!" << endl;
    return true;
}

// Admin registration - simplified using DatabaseManager
bool registerAdmin(const string& name, const string& email, AdminType adminType, const string& password) {

    // Only allow if no admin exists yet
    if (DatabaseManager::getAdminCount() > 0) {
        cout << "Error: Admin already exists. Use registerAdminByAdmin for additional admins." << endl;
        return false;
    }

    // Validate input
    if (name.empty()) {
        cout << "Error: Name cannot be empty." << endl;
        return false;
    }

    if (!isValidEmail(email)) {
        cout << "Error: Invalid email format." << endl;
        return false;
    }

    if (!isValidPassword(password)) {
        cout << "Error: Password must be at least 6 characters long." << endl;
        return false;
    }

    if (DatabaseManager::emailExists(email)) {
        cout << "Error: Email already exists." << endl;
        return false;
    }

    // Create and add new admin
    Admin newAdmin(email, password, name, adminType);
    DatabaseManager::addAdmin(newAdmin);

    cout << "Admin registered successfully!" << endl;
    return true;
}

bool registerAdminByAdmin(const string& name, const string& email, AdminType adminType, const string& password) {
    // Validate input
    if (name.empty()) {
        cout << "Error: Name cannot be empty." << endl;
        return false;
    }

    if (!isValidEmail(email)) {
        cout << "Error: Invalid email format." << endl;
        return false;
    }

    if (!isValidPassword(password)) {
        cout << "Error: Password must be at least 6 characters long." << endl;
        return false;
    }

    if (DatabaseManager::emailExists(email)) {
        cout << "Error: Email already exists." << endl;
        return false;
    }

    // Create and add new admin
    Admin newAdmin(email, password, name, adminType);
    DatabaseManager::addAdmin(newAdmin);

    cout << "Admin registration by admin successful!" << endl;
    return true;
}

// Universal login function - simplified using DatabaseManager
User* loginUser(const string& email, const string& password) {
    if (!isValidEmail(email)) {
        cout << "Error: Invalid email format." << endl;
        return nullptr;
    }

    // Check students
    Student* student = DatabaseManager::findStudentByEmail(email);
    if (student && student->getPassword() == password) {
        cout << "Student login successful! Welcome, " << student->getName() << endl;
        return student;
    }

    // Check teachers
    Teacher* teacher = DatabaseManager::findTeacherByEmail(email);
    if (teacher && teacher->getPassword() == password) {
        cout << "Teacher login successful! Welcome, " << teacher->getName() << endl;
        return teacher;
    }

    // Check admins
    Admin* admin = DatabaseManager::findAdminByEmail(email);
    if (admin && admin->getPassword() == password) {
        cout << "Admin login successful! Welcome, " << admin->getName() << endl;
        return admin;
    }

    // Check dining authorities
    DiningAuthority* diningAuth = DatabaseManager::findDiningAuthorityByEmail(email);
    if (diningAuth && diningAuth->getPassword() == password) {
        cout << "Dining Authority login successful! Welcome, " << diningAuth->getName() << endl;
        return diningAuth;
    }

    cout << "Error: Invalid email or password." << endl;
    return nullptr;
}

// Helper functions - simplified using DatabaseManager
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

// Display all registered users
void displayAllUsers() {
    cout << "\n=== ALL REGISTERED USERS ===" << endl;

    vector<Student> students = DatabaseManager::loadStudents();
    vector<Teacher> teachers = DatabaseManager::loadTeachers();
    vector<Admin> admins = DatabaseManager::loadAdmins();

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

    // Show database statistics
    DatabaseManager::displayDatabaseStats();
}

// Clear all authentication data
void clearAuthData() {
    DatabaseManager::clearAllData();
    cout << "All authentication data cleared." << endl;
}

// Auth class method implementations
Auth::Auth() {
    // Initialize database when Auth object is created
    DatabaseManager::initializeDatabase();
}

User* Auth::login(const string& email, const string& password) {
    return loginUser(email, password);
}

void Auth::logout() {
    // Reset any session-specific data if needed
}

bool Auth::registerStudent(const string &studentID, const string &name, const string &email,
                            int age, int classRoll, department dept, int batch,
                            Halls hall, const string &password) {
    return ::registerStudent(studentID, name, email, age, classRoll, dept, batch, hall, password);
}

bool Auth::registerTeacher(const string &name, const string &email, const string &department,
                            AcademicPosition rank, const string &password) {
    return ::registerTeacher(name, email, department, rank, password);
}

bool Auth::registerAdmin(const string &name, const string &email, AdminType adminType, const string &password) {
    return ::registerAdmin(name, email, adminType, password);
}

bool Auth::registerDiningAuthority(const string &name, const string &email, const string &hallName,
                                   const string &designation, const string &phoneNumber, const string &password) {
    // Validate input - simplified to match our refactored DiningAuthority class
    if (name.empty() || hallName.empty()) {
        cout << "Error: Name and Hall cannot be empty." << endl;
        return false;
    }
    if (!isValidEmail(email) || !isValidPassword(password)) {
        cout << "Error: Invalid email or password format." << endl;
        return false;
    }

    if (DatabaseManager::emailExists(email)) {
        cout << "Error: Email already exists." << endl;
        return false;
    }

    // Create new DiningAuthority with simplified constructor
    DiningAuthority newAuth(email, password, name, hallName);
    DatabaseManager::addDiningAuthority(newAuth);
    cout << "Dining authority registered successfully!" << endl;
    return true;
}

bool Auth::isEmailRegistered(const string &email) {
    return DatabaseManager::emailExists(email);
}

// Additional helper methods
DiningAuthority* Auth::getDiningAuthorityByEmail(const string& email) {
    return DatabaseManager::findDiningAuthorityByEmail(email);
}

void Auth::displayAllUsers() {
    ::displayAllUsers();

    // Also display dining authorities
    vector<DiningAuthority> diningAuthorities = DatabaseManager::loadDiningAuthorities();
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
}
