//
// Created by Md. Asif Khan on 11/8/25.
// University Management System Main Entry Point
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "../Database/DatabaseManager.h"
#include "../Users/student.h"
#include "../Users/teacher.h"
#include "../Users/Admin.h"
#include "../Users/user.h"
#include "../../Core/Utils/StringHelper.h"
using namespace std;

vector<Student> students;
vector<Teacher> teachers;


template<typename T>
void writeObjectToBinary(ofstream& out, const T& obj) {
    out.write(reinterpret_cast<const char*>(&obj), sizeof(obj));
}

template<typename T>
void readObjectFromBinary(ifstream& in, T& obj) {
    in.read(reinterpret_cast<char*>(&obj), sizeof(obj));
}


const string STUDENTS_DB = "student.dat";
const string TEACHERS_DB = "teacher.dat";

// Helper functions
void loadStudents() {
    ifstream file(STUDENTS_DB, ios::binary);
    if (file.is_open()) {
        students.clear();
        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        if (file) {
            students.reserve(size);
            for (size_t i = 0; i < size; i++) {
                Student student;
                readObjectFromBinary(file, student);
                students.push_back(student);
            }
        }
        file.close();
    }
}

void saveStudents() {
    ofstream file(STUDENTS_DB, ios::binary);
    if (file.is_open()) {
        size_t size = students.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        for (const auto& student : students) {
            writeObjectToBinary(file, student);
        }
        file.close();
    }
}

void loadTeachers() {
    ifstream file(TEACHERS_DB, ios::binary);
    if (file.is_open()) {
        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (file) {
            teachers.reserve(size);
            for (size_t i = 0; i < size; ++i) {
                Teacher teacher;
                readObjectFromBinary(file, teacher);
                    teachers.push_back(teacher);

            }
        }
        file.close();
    }
}

void saveTeachers() {
    ofstream file(TEACHERS_DB, ios::binary);
    if (file.is_open()) {
        size_t size = teachers.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        for (const auto& teacher : teachers) {
            writeObjectToBinary(file, teacher);
        }
        file.close();
    }
}

Student* authenticateStudent(const string& email, const string& password) {
    for (auto& student : students) {
        if (student.getEmail() == email && student.getPassword() == password) {
            return &student;
        }
    }
    return nullptr;
}

Teacher* authenticateTeacher(const string& email, const string& password) {
    for (auto& teacher : teachers) {
        if (teacher.getEmail() == email && teacher.getPassword() == password) {
            return &teacher;
        }
    }
    return nullptr;
}

void registerStudent() {
    string name, email, password, studentId, course;
    int age;

    cout << "=== Student Registration ===" << endl;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter your email: ";
    getline(cin, email);
    cout << "Enter your password: ";
    getline(cin, password);
    cout << "Enter your age: ";
    cin >> age;
    cout << "Enter your student ID: ";
    cin.ignore();
    getline(cin, studentId);
    cout << "Enter your course: ";
    getline(cin, course);

    // Check if email already exists
    for (const auto& student : students) {
        if (student.getEmail() == email) {
            cout << "Email already exists! Registration failed." << endl;
            return;
        }
    }

    Student newStudent(name, email, password, age, studentId, course);
    students.push_back(newStudent);
    saveStudents();
    cout << "Student registration successful!" << endl;
}

void registerTeacher() {
    string name, email, password, employeeId, department;
    int age;

    cout << "=== Teacher Registration ===" << endl;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter your email: ";
    getline(cin, email);
    cout << "Enter your password: ";
    getline(cin, password);
    cout << "Enter your age: ";
    cin >> age;
    cout << "Enter your employee ID: ";
    cin.ignore();
    getline(cin, employeeId);
    cout << "Enter your department: ";
    getline(cin, department);

    // Check if email already exists
    for (const auto& teacher : teachers) {
        if (teacher.getEmail() == email) {
            cout << "Email already exists! Registration failed." << endl;
            return;
        }
    }

    Teacher newTeacher(name, email, password, age, employeeId, department);
    teachers.push_back(newTeacher);
    saveTeachers();
    cout << "Teacher registration successful!" << endl;
}

void loginStudent() {
    string email, password;

    cout << "=== Student Login ===" << endl;
    cout << "Enter your email: ";
    cin.ignore();
    getline(cin, email);
    cout << "Enter your password: ";
    getline(cin, password);

    Student* student = authenticateStudent(email, password);
    if (student) {
        cout << "Login successful! Welcome, " << student->getName() << endl;
        // Add student dashboard functionality here
    } else {
        cout << "Invalid email or password!" << endl;
    }
}

void loginTeacher() {
    string email, password;

    cout << "=== Teacher Login ===" << endl;
    cout << "Enter your email: ";
    cin.ignore();
    getline(cin, email);
    cout << "Enter your password: ";
    getline(cin, password);

    Teacher* teacher = authenticateTeacher(email, password);
    if (teacher) {
        cout << "Login successful! Welcome, " << teacher->getName() << endl;
        // Add teacher dashboard functionality here
    } else {
        cout << "Invalid email or password!" << endl;
    }
}

int main() {
    // Load existing data
    loadStudents();
    loadTeachers();

    bool exit = false;
    while (!exit) {
        int choice;
        cout << "\n=== University Management System ===" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int choice2;
                cout << "\n=== Login ===" << endl;
                cout << "1. Student" << endl;
                cout << "2. Teacher" << endl;
                cout << "3. Back to main menu" << endl;
                cout << "Enter your choice: ";
                cin >> choice2;

                switch (choice2) {
                    case 1:
                        loginStudent();
                        break;
                    case 2:
                        loginTeacher();
                        break;
                    case 3:
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                        break;
                }
                break;
            }
            case 2: {
                int choice3;
                cout << "\n=== Registration ===" << endl;
                cout << "1. Student" << endl;
                cout << "2. Teacher" << endl;
                cout << "3. Back to main menu" << endl;
                cout << "Enter your choice: ";
                cin >> choice3;

                switch (choice3) {
                    case 1:
                        registerStudent();
                        break;
                    case 2:
                        registerTeacher();
                        break;
                    case 3:
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                        break;
                }
                break;
            }
            case 3:
                exit = true;
                cout << "Thank you for using University Management System!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
    }

    return 0;
}
