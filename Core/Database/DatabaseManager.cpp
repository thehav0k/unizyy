//
// Created by Md. Asif Khan on 11/8/25.
// DatabaseManager Implementation
//

#include "DatabaseManager.h"
#include "../Users/student.h"
#include "../Users/Teacher.h"
#include "../Users/Admin.h"
#include "../Users/DiningAuthority.h"
using namespace std;

// dat file gular location
const string DatabaseManager::STUDENTS_DB = "Database/students.dat";
const string DatabaseManager::TEACHERS_DB = "Database/teachers.dat";
const string DatabaseManager::ADMINS_DB = "Database/admins.dat";
const string DatabaseManager::DINING_AUTH_DB = "Database/dining_authorities.dat";
const string DatabaseManager::DATABASE_DIR = "Database";

// login korar shathe auto data gula file theke load hoye jabe
// then vector e save thakbe jate easily shob access kora jay
vector<Student> DatabaseManager::cachedStudents;
vector<Teacher> DatabaseManager::cachedTeachers;
vector<Admin> DatabaseManager::cachedAdmins;
vector<DiningAuthority> DatabaseManager::cachedDiningAuthorities;

// File-scope dataLoaded flag
static bool dataLoaded = false;

void DatabaseManager::initializeDatabase() {
    filesystem::create_directories(DATABASE_DIR);


    if (!filesystem::exists(STUDENTS_DB)) {
        ofstream file(STUDENTS_DB, ios::binary);
        file.close();
    }
    if (!filesystem::exists(TEACHERS_DB)) {
        ofstream file(TEACHERS_DB, ios::binary);
        file.close();
    }
    if (!filesystem::exists(ADMINS_DB)) {
        ofstream file(ADMINS_DB, ios::binary);
        file.close();
    }
    if (!filesystem::exists(DINING_AUTH_DB)) {
        ofstream file(DINING_AUTH_DB, ios::binary);
        file.close();
    }

    // Load data into cache
    if (!dataLoaded) {
        cachedStudents = loadStudents();
        cachedTeachers = loadTeachers();
        cachedAdmins = loadAdmins();
        cachedDiningAuthorities = loadDiningAuthorities();
        dataLoaded = true;
    }
}

// Student operations
vector<Student> DatabaseManager::loadStudents() {
    vector<Student> students;
    ifstream in(STUDENTS_DB, ios::binary);

    if (in.is_open()) {
        size_t count;
        if (in.read(reinterpret_cast<char*>(&count), sizeof(count))) {
            students.reserve(count);
            for (size_t i = 0; i < count; i++) {
                Student student;
                readObjectFromBinary(in, student);
                students.push_back(student);
            }
        }
        in.close();
    }
    return students;
}

void DatabaseManager::saveStudents(const vector<Student>& students) {
    ofstream out(STUDENTS_DB, ios::binary);
    if (out.is_open()) {
        size_t count = students.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto& student : students) {
            writeObjectToBinary(out, student);
        }
        out.close();
    }
    cachedStudents = students; // Update cache
}

bool DatabaseManager::addStudent(const Student& student) {
    cachedStudents.push_back(student);
    saveStudents(cachedStudents);
    return true;
}

Student* DatabaseManager::findStudentByEmail(const string& email) {
    for (auto& student : cachedStudents) {
        if (student.getEmail() == email) {
            return &student;
        }
    }
    return nullptr;
}

Student* DatabaseManager::findStudentByID(const string& studentID) {
    for (auto& student : cachedStudents) {
        if (student.getStudentID() == studentID) {
            return &student;
        }
    }
    return nullptr;
}

bool DatabaseManager::updateStudent(const string& studentID, const Student& updatedStudent) {
    for (size_t i = 0; i < cachedStudents.size(); i++) {
        if (cachedStudents[i].getStudentID() == studentID) {
            cachedStudents[i] = updatedStudent;
            saveStudents(cachedStudents);
            return true;
        }
    }
    return false;
}

// ekhane iterators use kora hoyeche
bool DatabaseManager::deleteStudent(const string& studentID) {
    for (auto it = cachedStudents.begin(); it != cachedStudents.end(); it++) {
        if (it->getStudentID() == studentID) {
            cachedStudents.erase(it);
            saveStudents(cachedStudents);
            return true;
        }
    }
    return false;
}

// Teacher operations
vector<Teacher> DatabaseManager::loadTeachers() {
    vector<Teacher> teachers;
    ifstream in(TEACHERS_DB, ios::binary);

    if (in.is_open()) {
        size_t count;
        if (in.read(reinterpret_cast<char*>(&count), sizeof(count))) {
            teachers.reserve(count);
            for (size_t i = 0; i < count; ++i) {
                Teacher teacher;
                readObjectFromBinary(in, teacher);
                teachers.push_back(teacher);
            }
        }
        in.close();
    }
    return teachers;
}

void DatabaseManager::saveTeachers(const vector<Teacher>& teachers) {
    ofstream out(TEACHERS_DB, ios::binary);
    if (out.is_open()) {
        size_t count = teachers.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto& teacher : teachers) {
            writeObjectToBinary(out, teacher);
        }
        out.close();
    }
    cachedTeachers = teachers; // Update cache
}

bool DatabaseManager::addTeacher(const Teacher& teacher) {
    cachedTeachers.push_back(teacher);
    saveTeachers(cachedTeachers);
    return true;
}

Teacher* DatabaseManager::findTeacherByEmail(const string& email) {
    for (auto& teacher : cachedTeachers) {
        if (teacher.getEmail() == email) {
            return &teacher;
        }
    }
    return nullptr;
}

bool DatabaseManager::updateTeacher(const string& email, const Teacher& updatedTeacher) {
    for (size_t i = 0; i < cachedTeachers.size(); ++i) {
        if (cachedTeachers[i].getEmail() == email) {
            cachedTeachers[i] = updatedTeacher;
            saveTeachers(cachedTeachers);
            return true;
        }
    }
    return false;
}

bool DatabaseManager::deleteTeacher(const string& email) {
    for (auto it = cachedTeachers.begin(); it != cachedTeachers.end(); ++it) {
        if (it->getEmail() == email) {
            cachedTeachers.erase(it);
            saveTeachers(cachedTeachers);
            return true;
        }
    }
    return false;
}

// Admin operations
vector<Admin> DatabaseManager::loadAdmins() {
    vector<Admin> admins;
    ifstream in(ADMINS_DB, ios::binary);

    if (in.is_open()) {
        size_t count;
        if (in.read(reinterpret_cast<char*>(&count), sizeof(count))) {
            admins.reserve(count);
            for (size_t i = 0; i < count; ++i) {
                Admin admin;
                readObjectFromBinary(in, admin);
                admins.push_back(admin);
            }
        }
        in.close();
    }
    return admins;
}

void DatabaseManager::saveAdmins(const vector<Admin>& admins) {
    ofstream out(ADMINS_DB, ios::binary);
    if (out.is_open()) {
        size_t count = admins.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto& admin : admins) {
            writeObjectToBinary(out, admin);
        }
        out.close();
    }
    cachedAdmins = admins; // Update cache
}

bool DatabaseManager::addAdmin(const Admin& admin) {
    cachedAdmins.push_back(admin);
    saveAdmins(cachedAdmins);
    return true;
}

Admin* DatabaseManager::findAdminByEmail(const string& email) {
    for (auto& admin : cachedAdmins) {
        if (admin.getEmail() == email) {
            return &admin;
        }
    }
    return nullptr;
}

bool DatabaseManager::updateAdmin(const string& email, const Admin& updatedAdmin) {
    for (size_t i = 0; i < cachedAdmins.size(); i++) {
        if (cachedAdmins[i].getEmail() == email) {
            cachedAdmins[i] = updatedAdmin;
            saveAdmins(cachedAdmins);
            return true;
        }
    }
    return false;
}

bool DatabaseManager::deleteAdmin(const string& email) {
    for (auto it = cachedAdmins.begin(); it != cachedAdmins.end(); ++it) {
        if (it->getEmail() == email) {
            cachedAdmins.erase(it);
            saveAdmins(cachedAdmins);
            return true;
        }
    }
    return false;
}

// DiningAuthority operations
vector<DiningAuthority> DatabaseManager::loadDiningAuthorities() {
    vector<DiningAuthority> authorities;
    ifstream in(DINING_AUTH_DB, ios::binary);
    if (in.is_open()) {
        size_t count;
        if (in.read(reinterpret_cast<char*>(&count), sizeof(count))) {
            authorities.reserve(count);
            for (size_t i = 0; i < count; i++) {
                DiningAuthority auth;
                readObjectFromBinary(in, auth);
                authorities.push_back(auth);
            }
        }
        in.close();
    }
    return authorities;
}

void DatabaseManager::saveDiningAuthorities(const vector<DiningAuthority>& authorities) {
    ofstream out(DINING_AUTH_DB, ios::binary);
    if (out.is_open()) {
        size_t count = authorities.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        for (const auto& auth : authorities) {
            writeObjectToBinary(out, auth);
        }
        out.close();
    }
    cachedDiningAuthorities = authorities;
}

bool DatabaseManager::addDiningAuthority(const DiningAuthority& authority) {
    cachedDiningAuthorities.push_back(authority);
    saveDiningAuthorities(cachedDiningAuthorities);
    return true;
}

DiningAuthority* DatabaseManager::findDiningAuthorityByEmail(const string& email) {
    for (auto& auth : cachedDiningAuthorities) {
        if (auth.getEmail() == email) {
            return &auth;
        }
    }
    return nullptr;
}

bool DatabaseManager::updateDiningAuthority(const string& email, const DiningAuthority& updatedAuthority) {
    for (size_t i = 0; i < cachedDiningAuthorities.size(); ++i) {
        if (cachedDiningAuthorities[i].getEmail() == email) {
            cachedDiningAuthorities[i] = updatedAuthority;
            saveDiningAuthorities(cachedDiningAuthorities);
            return true;
        }
    }
    return false;
}

bool DatabaseManager::deleteDiningAuthority(const string& email) {
    for (auto it = cachedDiningAuthorities.begin(); it != cachedDiningAuthorities.end(); ++it) {
        if (it->getEmail() == email) {
            cachedDiningAuthorities.erase(it);
            saveDiningAuthorities(cachedDiningAuthorities);
            return true;
        }
    }
    return false;
}

// Utility operations
bool DatabaseManager::emailExists(const string& email) {
    return findStudentByEmail(email) != nullptr ||
           findTeacherByEmail(email) != nullptr ||
           findAdminByEmail(email) != nullptr ||
           findDiningAuthorityByEmail(email) != nullptr;
}

bool DatabaseManager::studentIDExists(const string& studentID) {
    return findStudentByID(studentID) != nullptr;
}

void DatabaseManager::clearAllData() {
    cachedStudents.clear();
    cachedTeachers.clear();
    cachedAdmins.clear();
    cachedDiningAuthorities.clear();

    saveStudents(cachedStudents);
    saveTeachers(cachedTeachers);
    saveAdmins(cachedAdmins);
    saveDiningAuthorities(cachedDiningAuthorities);
}

size_t DatabaseManager::getStudentCount() {
    return cachedStudents.size();
}

size_t DatabaseManager::getTeacherCount() {
    return cachedTeachers.size();
}

size_t DatabaseManager::getAdminCount() {
    return cachedAdmins.size();
}

size_t DatabaseManager::getDiningAuthorityCount() {
    return cachedDiningAuthorities.size();
}

void DatabaseManager::displayDatabaseStats() {
    cout << "\n=== DATABASE STATISTICS ===" << endl;
    cout << "Students: " << getStudentCount() << endl;
    cout << "Teachers: " << getTeacherCount() << endl;
    cout << "Admins: " << getAdminCount() << endl;
    cout << "Dining Authorities: " << getDiningAuthorityCount() << endl;
    cout << "Total Users: " << (getStudentCount() + getTeacherCount() + getAdminCount() + getDiningAuthorityCount()) << endl;
}
