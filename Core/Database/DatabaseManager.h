//
// Created by Md. Asif Khan on 11/8/25.
//

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "../../System/Modules/Meal/meal.h"
#include"NoticeManager.h"
using namespace std;


// forward declare jate ei class e use kora
class Notice;
class Student;
class Teacher;
class Admin;
class DiningAuthority;

// ekta static class, object bananor dorkar nai
// anyway namespace o use kora jaito but static class sounds cool
class DatabaseManager {
private:
    static const string STUDENTS_DB;
    static const string TEACHERS_DB;
    static const string ADMINS_DB;
    static const string DATABASE_DIR;
    static const string DINING_AUTH_DB;
    static const string ACTIVE_TOKENS_DB;
    static const string USED_TOKENS_DB;
    static const string REVIEWS_DB;
    static const string MEALS_DB;
    static const string NOTICES_DB;

    // file er sob data vector gulay joma hbe program shurur por
    // style hocche load data to vector-> modify sob in vector -> tarpor abr save in file
    static vector<Student> cachedStudents;
    static vector<Teacher> cachedTeachers;
    static vector<Admin> cachedAdmins;
    static vector<DiningAuthority> cachedDiningAuthorities;
    static vector<MealToken> cachedActiveTokens;
    static vector<MealToken> cachedUsedTokens;
    static vector<MealReview> cachedReviews;
    static vector<Meal> cachedMeals;
    static vector<Notice> cachedNotices;

public:
    // direct object writing & reading er jnno template method
    // jate jekono object same method diye read write kora jay
    // Man I love templates, code size 70% komaya diche
    template<typename T>
    static void writeObjectToBinary(ofstream& out, const T& obj);
    template<typename T>
    static void readObjectFromBinary(ifstream& in, T& obj);

    // Generic template methods for common database operations
    template<typename T>
    static vector<T> loadObjects(const string& filePath);

    template<typename T>
    static void saveObjects(const vector<T>& objects, const string& filePath);
    // eita ektu tricky function er moddhe function parameter hisebe pass kora hoise
    // func jekono method hote pare -> getemail/id etc
    // func object er pointer return kore
    template<typename T, typename KeyType>
    static T* findObjectByKey(vector<T>& container,const KeyType& key,KeyType (T::*func)() const);

    template<typename T>
    static bool addObject(vector<T>& cache, const T& object, const string& filePath);

    template<typename T, typename KeyType>
    static bool updateObject(vector<T>& cache, const KeyType& key,const T& updatedObject, const string& filePath,KeyType (T::*func )() const);

    template<typename T, typename KeyType>
    static bool deleteObject(vector<T>& cache, const KeyType& key,const string& filePath,KeyType (T::*func )() const);

    // Database initialize hobe mane sob file theke vector e cache hisebe laod hbe
    static void initializeDatabase();

    // Student
    static vector<Student> loadStudents();
    static void saveStudents(const vector<Student>& students);
    static bool addStudent(const Student& student);
    static bool updateStudent(const string& studentID, const Student& updatedStudent);
    static bool deleteStudent(const string& studentID);
    static Student* findStudentByEmail(const string& email);
    static Student* findStudentByID(const string& studentID);

    // Teacher
    static vector<Teacher> loadTeachers();
    static void saveTeachers(const vector<Teacher>& teachers);
    static bool addTeacher(const Teacher& teacher);
    static bool updateTeacher(const string& email, const Teacher& updatedTeacher);
    static bool deleteTeacher(const string& email);
    static Teacher* findTeacherByEmail(const string& email);

    // Admin
    static vector<Admin> loadAdmins();
    static void saveAdmins(const vector<Admin>& admins);
    static bool addAdmin(const Admin& admin);
    static Admin* findAdminByEmail(const string& email);
    static bool updateAdmin(const string& email, const Admin& updatedAdmin);
    static bool deleteAdmin(const string& email);

    // DiningAuthority
    static vector<DiningAuthority> loadDiningAuthorities();
    static void saveDiningAuthorities(const vector<DiningAuthority>& authorities);
    static bool addDiningAuthority(const DiningAuthority& authority);
    static DiningAuthority* findDiningAuthorityByEmail(const string& email);
    static bool updateDiningAuthority(const string& email, const DiningAuthority& updatedAuthority);
    static bool deleteDiningAuthority(const string& email);

    // habijabi
    static bool emailExists(const string& email);
    static bool studentIDExists(const string& studentID);
    static void clearAllData();
    // static void backupData(const string& backupDir);
    // static bool restoreData(const string& backupDir);

    // Tokens
    static vector<MealToken> loadActiveTokens();
    static void saveActiveTokens(const vector<MealToken>& tokens);
    static bool addActiveToken(const MealToken& token);
    static bool updateActiveToken(const string& tokenID, const MealToken& updatedToken);
    static bool deleteActiveToken(const string& tokenID);
    static MealToken* findActiveTokenByID(const string& tokenID);

    // Used Token operations
    static vector<MealToken> loadUsedTokens();
    static void saveUsedTokens(const vector<MealToken>& tokens);
    static bool addUsedToken(const MealToken& token);
    static bool updateUsedToken(const string& tokenID, const MealToken& updatedToken);
    static bool deleteUsedToken(const string& tokenID);
    static MealToken* findUsedTokenByID(const string& tokenID);

    // Review
    static vector<MealReview> loadReviews();
    static void saveReviews(const vector<MealReview>& reviews);
    static bool addReview(const MealReview& review);
    static bool updateReview(const string& reviewID, const MealReview& updatedReview);
    static bool deleteReview(const string& reviewID);
    static MealReview* findReviewByID(const string& reviewID);

    // Meal
    static vector<Meal> loadMeals();
    static void saveMeals(const vector<Meal>& meals);
    static bool addMeal(const Meal& meal);
    static bool updateMeal(const string& mealKey, const Meal& updatedMeal);
    static bool deleteMeal(const string& mealKey);
    static vector<Meal> getMealsByHall(const string& hall);
    static vector<Meal> getMealsByDate(const string& date);
    static vector<Meal> getMealsByType(MealType type);

    //Notice
    static  vector<Notice> loadNotices();
    static void saveNotices(const  vector<Notice>& notices);
    static bool addNotice(const Notice& notice);
    static bool updateNotice(int index, const Notice& updatedNotice);


    // Database statistics
    static size_t getStudentCount();
    static size_t getTeacherCount();
    static size_t getAdminCount();
    static size_t getSystemAdminCount();
    static size_t getDiningAuthorityCount();
    static void displayDatabaseStats();
};

// single object write read korar method
// onek jaygay use hbe
template<typename T>
void DatabaseManager::writeObjectToBinary(ofstream& out, const T& obj) {
    out.write(reinterpret_cast<const char*>(&obj), sizeof(obj));
}

template<typename T>
void DatabaseManager::readObjectFromBinary(ifstream& in, T& obj) {
    in.read(reinterpret_cast<char*>(&obj), sizeof(obj));
}

// file theke object load kore vector e anar method
// file er first e ekta integer thakbe jeta size( mane koyat object) ache thakbe
// tarpor ekta ekta kore for loop diyei object read kora jbe
template<typename T>
vector<T> DatabaseManager::loadObjects(const string& filePath) {
    vector<T> objects;
    ifstream in(filePath, ios::binary);

    if (in.is_open()) {
        size_t count;
        if (in.read(reinterpret_cast<char*>(&count), sizeof(count))) {
            objects.reserve(count);
            for (size_t i = 0; i < count; i++) {
                T object;
                readObjectFromBinary(in, object);
                objects.push_back(object);
            }
        }
        in.close();
    }
    return objects;
}


// vector theke file e save korar method
template<typename T>
void DatabaseManager::saveObjects(const vector<T>& objects, const string& filePath) {
    ofstream out(filePath, ios::binary);
    if (out.is_open()) {
        size_t count = objects.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto& object : objects) {
            writeObjectToBinary(out, object);
        }
        out.close();
    }
}

// kono data find korar method
// parameter hisebe vector, ekta function, ar oi data thakbe
// pointer return korbe
template<typename T, typename KeyType>
T* DatabaseManager::findObjectByKey(vector<T>& container,const KeyType& key,KeyType (T::*func)() const) {
    for (auto& object : container) {
        if ((object.*func)() == key) {
            return &object;
        }
    }
    return nullptr;
}

// object add , just pushback and save
template<typename T>
bool DatabaseManager::addObject(vector<T>& cache, const T& object, const string& filePath) {
    try {
        cache.push_back(object);
        saveObjects(cache, filePath);
        return true;
    } catch (const exception&) {
        // save na hole cancel
        if (!cache.empty()) {
            cache.pop_back();
        }
        return false;
    }
}

// update jemon password change korar method, change in object and save to file
template<typename T, typename KeyType>
bool DatabaseManager::updateObject(vector<T>& cache, const KeyType& key,const T& updatedObject, const string& filePath,KeyType (T::*func )() const) {
    try {
        for (size_t i = 0; i < cache.size(); i++) {
            if ((cache[i].*func )() == key) {
                cache[i] = updatedObject;
                saveObjects(cache, filePath);
                return true;
            }
        }
        return false; // Object nai
    } catch (const exception&) {
        return false;
    }
}

// delete o same way te, delete in vecttor and save to file
template<typename T, typename KeyType>
bool DatabaseManager::deleteObject(vector<T>& cache, const KeyType& key,const string& filePath,KeyType (T::*func )() const) {
    try {
        for (auto it = cache.begin(); it != cache.end(); ++it) {
            if (((*it).*func )() == key) {
                cache.erase(it);
                saveObjects(cache, filePath);
                return true;
            }
        }
        return false;
    } catch (const exception&) {
        return false;
    }
}

#endif //DATABASEMANAGER_H
