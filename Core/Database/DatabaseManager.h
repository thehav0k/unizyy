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

    // Generic collection methods for any object type
    template<typename T>
    static void saveObjects(const vector<T>& objects, const string& filename);

    template<typename T>
    static void loadObjects(vector<T>& objects, const string& filename);

    template<typename T>
    static bool saveObject(const T& obj, const string& filename);

    template<typename T>
    static bool updateObject(const T& obj, const string& filename);

    template<typename T>
    static bool deleteObject(const T& obj, const string& filename);

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
    static size_t getSystemAdminCount();
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

// Generic collection methods - save vector of objects with count header
template<typename T>
void DatabaseManager::saveObjects(const vector<T>& objects, const string& filename) {
    filesystem::create_directories(DATABASE_DIR);
    string filepath = DATABASE_DIR + "/" + filename + ".dat";
    string tmpfile = filepath + ".tmp";

    ofstream out(tmpfile, ios::binary | ios::trunc);
    if (!out.is_open()) return;

    size_t count = objects.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& obj : objects) {
        writeObjectToBinary(out, obj);
    }
    out.flush();
    out.close();

    // Atomic replace
    std::error_code ec;
    filesystem::rename(tmpfile, filepath, ec);
    if (ec) {
        filesystem::copy_file(tmpfile, filepath, filesystem::copy_options::overwrite_existing, ec);
        filesystem::remove(tmpfile, ec);
    }
}

// Generic collection methods - load vector of objects with count header
template<typename T>
void DatabaseManager::loadObjects(vector<T>& objects, const string& filename) {
    objects.clear();
    filesystem::create_directories(DATABASE_DIR);
    string filepath = DATABASE_DIR + "/" + filename + ".dat";

    if (!filesystem::exists(filepath)) return;

    ifstream in(filepath, ios::binary);
    if (!in.is_open()) return;

    // Get file size to validate count header
    in.seekg(0, ios::end);
    streampos fileSize = in.tellg();
    in.seekg(0, ios::beg);

    if (fileSize < static_cast<streampos>(sizeof(size_t))) {
        // File too small to have valid count header
        return;
    }

    size_t count;
    if (!in.read(reinterpret_cast<char*>(&count), sizeof(count))) return;

    // Validate count makes sense for file size
    streampos expectedSize = static_cast<streampos>(sizeof(size_t)) +
                            static_cast<streampos>(count) * static_cast<streampos>(sizeof(T));

    if (expectedSize != fileSize || count > 10000) {
        // File format doesn't match or count is unreasonable
        // This might be an old format file - just clear and return
        objects.clear();
        return;
    }

    objects.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        T obj;
        if (!in.read(reinterpret_cast<char*>(&obj), sizeof(T))) {
            // If read fails, truncate vector and break
            objects.resize(i);
            break;
        }
        objects.push_back(obj);
    }
}

// Save single object by appending to file
template<typename T>
bool DatabaseManager::saveObject(const T& obj, const string& filename) {
    filesystem::create_directories(DATABASE_DIR);
    string filepath = DATABASE_DIR + "/" + filename + ".dat";

    ofstream out(filepath, ios::binary | ios::app);
    if (!out.is_open()) return false;

    writeObjectToBinary(out, obj);
    return true;
}

// Update object - requires loading all, finding match, and rewriting
template<typename T>
bool DatabaseManager::updateObject(const T& obj, const string& filename) {
    vector<T> objects;
    loadObjects(objects, filename);

    // For generic update, we can't know how to match objects
    // This would need specialization for specific types
    // For now, just save all objects back
    saveObjects(objects, filename);
    return true;
}

// Delete object - requires loading all, removing match, and rewriting
template<typename T>
bool DatabaseManager::deleteObject(const T& obj, const string& filename) {
    vector<T> objects;
    loadObjects(objects, filename);

    // For generic delete, we can't know how to match objects
    // This would need specialization for specific types
    // For now, just save all objects back
    saveObjects(objects, filename);
    return true;
}

#endif //DATABASEMANAGER_H
