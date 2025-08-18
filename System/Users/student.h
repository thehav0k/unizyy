//
// Created by Md. Asif Khan on 8/8/25.
//

#ifndef STUDENT_H
#define STUDENT_H
#include "user.h"
#include <string>
#include <vector>
#include <fstream>
#include "../../Core/Utils/StringHelper.h"

#include "../../Core/Models/department.h"
#include "../../Core/Models/hall.h"
using namespace std;


class Student : public User {
private:
    char name[60];
    int age;
    char studentID[15];
    int classRoll;
    department dept;
    int batch;
    Halls hall;
    int Gender; // 1 for male // Zero for female // Only two gender
    double balance; // balace in bdt // initially 0.0

public:
    Student(const string &email, const string &password, const string &name, int age, const string &studentID,
            int classRoll, department dept, int batch, Halls hall)
        : User(email, password), age(age), classRoll(classRoll),
          dept(dept), batch(batch), hall(hall) {

        // store as char array
        //use as string
        // thats why conversion
        StringHelper::stringToCharArray(name, this->name);
        StringHelper::stringToCharArray(studentID, this->studentID);

        // hall onujayi gender auto
        this->Gender = getHallGender(hall);
        this->balance = 6000.0;
    }

    // Default constructor
    Student() : User(), age(0), classRoll(0), dept(department::Department_of_Computer_Science_and_Engineering),
                batch(0), hall(Halls::Al_Beruni_Hall), Gender(1), balance(0.0) {
        name[0] = '\0';
        studentID[0] = '\0';
    }

    // Getters && setters
    string getName() const {
        return StringHelper::charArrayToString(name);
    }

    void setName(const string &name) {
        StringHelper::stringToCharArray(name, this->name);
    }

    int getAge() const {
        return age;
    }

    void setAge(int age) {
        Student::age = age;
    }

    string getStudentID() const {
        return StringHelper::charArrayToString(studentID);
    }

    void setStudentID(const string &studentID) {
        StringHelper::stringToCharArray(studentID, this->studentID);
    }

    int getClassRoll() const {
        return classRoll;
    }

    void setClassRoll(int classRoll) {
        Student::classRoll = classRoll;
    }

    department getDepartment() const {
        return dept;
    }

    void setDepartment(department dept) {
        Student::dept = dept;
    }

    int getBatch() const {
        return batch;
    }

    void setBatch(int batch) {
        Student::batch = batch;
    }

    Halls getHall() const {
        return hall;
    }

    void setHall(Halls hall) {
        Student::hall = hall;
        // Update gender when hall is changed
        this->Gender = getHallGender(hall);
    }

    int getGender() const {
        return Gender;
    }

    void setGender(int gender) {
        Student::Gender = gender;
    }

    string getGenderString() const {
        return (Gender == 1) ? "Male" : "Female";
    }

    double getBalance() const {
        return balance;
    }

    void setBalance(double balance) {
        Student::balance = balance;
    }

    // sob function er prototype
    // sob info dsiplay
    void display() const;
    void ChangePassword(const string &newPassword);
    void BuyMealToken();
    void TakeMeal();
    void ReviewMeal();


    void ViewResult();

    void ViewNotice();

    void ViewClassSchedule();

    void ViewTransportSchedule();

    // database er kaj sob DatabaseManager class korbe
    // ekhane just function call
    static vector<Student> loadAllStudents();
    static bool addStudentToDB(const Student& student);
    static bool updateStudentInDB(const string& studentID, const Student& updatedStudent);
    static bool deleteStudentFromDB(const string& studentID);
    static Student* findStudentByEmail(const string& email);
    static Student* findStudentByID(const string& studentID);
};
#endif //STUDENT_H
