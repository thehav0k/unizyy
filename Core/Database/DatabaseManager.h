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
    template<typename T>
    static void writeObjectToBinary(ofstream& out, const T& obj);
    template<typename T>
    static void readObjectFromBinary(ifstream& in, T& obj);

    // Generic template methods for common database operations
    template<typename T>
    static vector<T> loadObjects(const string& filePath);

    template<typename T>
    static void saveObjects(const vector<T>& objects, const string& filePath);

    template<typename T, typename KeyType>
    static T* findObjectByKey(vector<T>& container,
                             const KeyType& key,
                             KeyType (T::*getter)() const);

    template<typename T>
    static bool addObject(vector<T>& cache, const T& object, const string& filePath);

    template<typename T, typename KeyType>
    static bool updateObject(vector<T>& cache, const KeyType& key,
                            const T& updatedObject, const string& filePath,
                            KeyType (T::*getter)() const);

    template<typename T, typename KeyType>
    static bool deleteObject(vector<T>& cache, const KeyType& key,
                            const string& filePath,
                            KeyType (T::*getter)() const);

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

    // Active Token operations
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

    // Review operations
    static vector<MealReview> loadReviews();
    static void saveReviews(const vector<MealReview>& reviews);
    static bool addReview(const MealReview& review);
    static bool updateReview(const string& reviewID, const MealReview& updatedReview);
    static bool deleteReview(const string& reviewID);
    static MealReview* findReviewByID(const string& reviewID);

    // Meal operations
    static vector<Meal> loadMeals();
    static void saveMeals(const vector<Meal>& meals);
    static bool addMeal(const Meal& meal);
    static bool updateMeal(const string& mealKey, const Meal& updatedMeal);
    static bool deleteMeal(const string& mealKey);
    static vector<Meal> getMealsByHall(const string& hall);
    static vector<Meal> getMealsByDate(const string& date);
    static vector<Meal> getMealsByType(MealType type);

    //Notice opreations
    static std::vector<Notice> loadNotices();
    static void saveNotices(const std::vector<Notice>& notices);
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

// General template for loading objects
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


// General template for saving objects
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

// Generic finder template using member function pointer
template<typename T, typename KeyType>
T* DatabaseManager::findObjectByKey(vector<T>& container,
                                   const KeyType& key,
                                   KeyType (T::*getter)() const) {
    for (auto& object : container) {
        if ((object.*getter)() == key) {
            return &object;
        }
    }
    return nullptr;
}

// Generic add object template
template<typename T>
bool DatabaseManager::addObject(vector<T>& cache, const T& object, const string& filePath) {
    try {
        cache.push_back(object);
        saveObjects(cache, filePath);
        return true;
    } catch (const exception&) {
        // If saving fails, remove the object from cache and return false
        if (!cache.empty()) {
            cache.pop_back();
        }
        return false;
    }
}

// Generic update object template
template<typename T, typename KeyType>
bool DatabaseManager::updateObject(vector<T>& cache, const KeyType& key,
                                  const T& updatedObject, const string& filePath,
                                  KeyType (T::*getter)() const) {
    try {
        for (size_t i = 0; i < cache.size(); i++) {
            if ((cache[i].*getter)() == key) {
                cache[i] = updatedObject;
                saveObjects(cache, filePath);
                return true;
            }
        }
        return false; // Object not found
    } catch (const exception&) {
        return false;
    }
}

// Generic delete object template
template<typename T, typename KeyType>
bool DatabaseManager::deleteObject(vector<T>& cache, const KeyType& key,
                                  const string& filePath,
                                  KeyType (T::*getter)() const) {
    try {
        for (auto it = cache.begin(); it != cache.end(); it++) {
            if (((*it).*getter)() == key) {
                cache.erase(it);
                saveObjects(cache, filePath);
                return true;
            }
        }
        return false; // Object not found
    } catch (const exception&) {
        return false;
    }
}

#endif //DATABASEMANAGER_H
