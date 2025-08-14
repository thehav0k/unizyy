//
// Created by Md. Asif Khan on 11/8/25.
// Centralized Database Manager for all file operations
//

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std;
// agei declare jate use kora jay
// pore define kora jabe nij nij file e
class Student;
class Teacher;
class Admin;
class DiningAuthority;

class DatabaseManager {
private:
    // Database file paths
    static const string STUDENTS_DB;
    static const string TEACHERS_DB;
    static const string ADMINS_DB;
    static const string DATABASE_DIR;
    // DiningAuthority database file path
    static const string DINING_AUTH_DB;

    // Static cache for loaded data
    static vector<Student> cachedStudents;
    static vector<Teacher> cachedTeachers;
    static vector<Admin> cachedAdmins;
    static vector<DiningAuthority> cachedDiningAuthorities;

public:
    // Generic template methods for direct object writing - NOW PUBLIC
    template<typename T>
    static void writeObjectToBinary(ofstream& out, const T& obj);

    template<typename T>
    static void readObjectFromBinary(ifstream& in, T& obj);

    // Database initialization
    static void initializeDatabase();

    // Student operations
    static vector<Student> loadStudents();
    static void saveStudents(const vector<Student>& students);
    static bool addStudent(const Student& student);
    static bool updateStudent(const string& studentID, const Student& updatedStudent);
    static bool deleteStudent(const string& studentID);
    static Student* findStudentByEmail(const string& email);
    static Student* findStudentByID(const string& studentID);

    // Teacher operations
    static vector<Teacher> loadTeachers();
    static void saveTeachers(const vector<Teacher>& teachers);
    static bool addTeacher(const Teacher& teacher);
    static bool updateTeacher(const string& email, const Teacher& updatedTeacher);
    static bool deleteTeacher(const string& email);
    static Teacher* findTeacherByEmail(const string& email);

    // Admin operations
    static vector<Admin> loadAdmins();
    static void saveAdmins(const vector<Admin>& admins);
    static bool addAdmin(const Admin& admin);
    static Admin* findAdminByEmail(const string& email);
    static bool updateAdmin(const string& email, const Admin& updatedAdmin);
    static bool deleteAdmin(const string& email);

    // DiningAuthority operations
    static vector<DiningAuthority> loadDiningAuthorities();
    static void saveDiningAuthorities(const vector<DiningAuthority>& authorities);
    static bool addDiningAuthority(const DiningAuthority& authority);
    static DiningAuthority* findDiningAuthorityByEmail(const string& email);
    static bool updateDiningAuthority(const string& email, const DiningAuthority& updatedAuthority);
    static bool deleteDiningAuthority(const string& email);

    // Helper operations
    static bool emailExists(const string& email);
    static bool studentIDExists(const string& studentID);
    static void clearAllData();
    static void backupData(const string& backupDir);
    static bool restoreData(const string& backupDir);

    // Database statistics
    static size_t getStudentCount();
    static size_t getTeacherCount();
    static size_t getAdminCount();
    static size_t getDiningAuthorityCount();
    static void displayDatabaseStats();
};

// Template implementations (must be in header)
// Write an object to a binary file
template<typename T>
void DatabaseManager::writeObjectToBinary(ofstream& out, const T& obj) {
    out.write(reinterpret_cast<const char*>(&obj), sizeof(obj));
}

// Read an object from a binary file
template<typename T>
void DatabaseManager::readObjectFromBinary(ifstream& in, T& obj) {
    in.read(reinterpret_cast<char*>(&obj), sizeof(obj));
}

#endif //DATABASEMANAGER_H
