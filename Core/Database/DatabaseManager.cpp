//
// Created by Md. Asif Khan on 11/8/25.
// DatabaseManager Implementation
//

#include "DatabaseManager.h"
#include "../Users/student.h"
#include "../Users/Teacher.h"
#include "../Users/Admin.h"
#include "../Users/DiningAuthority.h"
#include "../../System/Modules/Meal/meal.h"
using namespace std;

// dat file gular location
const string DatabaseManager::STUDENTS_DB = "Database/students.dat";
const string DatabaseManager::TEACHERS_DB = "Database/teachers.dat";
const string DatabaseManager::ADMINS_DB = "Database/admins.dat";
const string DatabaseManager::DINING_AUTH_DB = "Database/dining_authorities.dat";
const string DatabaseManager::ACTIVE_TOKENS_DB = "Database/active_tokens.dat";
const string DatabaseManager::USED_TOKENS_DB = "Database/used_tokens.dat";
const string DatabaseManager::REVIEWS_DB = "Database/meal_reviews.dat";
const string DatabaseManager::MEALS_DB = "Database/meals.dat";
const string DatabaseManager::DATABASE_DIR = "Database";

// login korar shathe auto data gula file theke load hoye jabe
// then vector e save thakbe jate easily shob access kora jay
vector<Student> DatabaseManager::cachedStudents;
vector<Teacher> DatabaseManager::cachedTeachers;
vector<Admin> DatabaseManager::cachedAdmins;
vector<DiningAuthority> DatabaseManager::cachedDiningAuthorities;
vector<MealToken> DatabaseManager::cachedActiveTokens;
vector<MealToken> DatabaseManager::cachedUsedTokens;
vector<MealReview> DatabaseManager::cachedReviews;
vector<Meal> DatabaseManager::cachedMeals;

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
    if (!filesystem::exists(ACTIVE_TOKENS_DB)) {
        ofstream file(ACTIVE_TOKENS_DB, ios::binary);
        file.close();
    }
    if (!filesystem::exists(USED_TOKENS_DB)) {
        ofstream file(USED_TOKENS_DB, ios::binary);
        file.close();
    }
    if (!filesystem::exists(REVIEWS_DB)) {
        ofstream file(REVIEWS_DB, ios::binary);
        file.close();
    }
    if (!filesystem::exists(MEALS_DB)) {
        ofstream file(MEALS_DB, ios::binary);
        file.close();
    }

    if (!dataLoaded) {
        cachedStudents = loadStudents();
        cachedTeachers = loadTeachers();
        cachedAdmins = loadAdmins();
        cachedDiningAuthorities = loadDiningAuthorities();
        cachedActiveTokens = loadActiveTokens();
        cachedUsedTokens = loadUsedTokens();
        cachedReviews = loadReviews();
        cachedMeals = loadMeals();
        dataLoaded = true;
    }
}

vector<Student> DatabaseManager::loadStudents() {
    return loadObjects<Student>(STUDENTS_DB);
}

void DatabaseManager::saveStudents(const vector<Student>& students) {
    saveObjects(students, STUDENTS_DB);
    cachedStudents = students;
}

bool DatabaseManager::addStudent(const Student& student) {
    return addObject(cachedStudents, student, STUDENTS_DB);
}

Student* DatabaseManager::findStudentByEmail(const string& email) {
    return findObjectByKey<Student, string>(cachedStudents, email, &Student::getEmail);
}

Student* DatabaseManager::findStudentByID(const string& studentID) {
    return findObjectByKey<Student, string>(cachedStudents, studentID, &Student::getStudentID);
}

bool DatabaseManager::updateStudent(const string& studentID, const Student& updatedStudent) {
    return updateObject<Student, string>(cachedStudents, studentID, updatedStudent, STUDENTS_DB, &Student::getStudentID);
}

bool DatabaseManager::deleteStudent(const string& studentID) {
    return deleteObject<Student, string>(cachedStudents, studentID, STUDENTS_DB, &Student::getStudentID);
}

// Teacher operations
vector<Teacher> DatabaseManager::loadTeachers() {
    return loadObjects<Teacher>(TEACHERS_DB);
}

void DatabaseManager::saveTeachers(const vector<Teacher>& teachers) {
    saveObjects(teachers, TEACHERS_DB);
    cachedTeachers = teachers;
}

bool DatabaseManager::addTeacher(const Teacher& teacher) {
    return addObject(cachedTeachers, teacher, TEACHERS_DB);
}

Teacher* DatabaseManager::findTeacherByEmail(const string& email) {
    return findObjectByKey<Teacher, string>(cachedTeachers, email, &Teacher::getEmail);
}

bool DatabaseManager::updateTeacher(const string& email, const Teacher& updatedTeacher) {
    return updateObject<Teacher, string>(cachedTeachers, email, updatedTeacher, TEACHERS_DB, &Teacher::getEmail);
}

bool DatabaseManager::deleteTeacher(const string& email) {
    return deleteObject<Teacher, string>(cachedTeachers, email, TEACHERS_DB, &Teacher::getEmail);
}

// Admin operations
vector<Admin> DatabaseManager::loadAdmins() {
    return loadObjects<Admin>(ADMINS_DB);
}

void DatabaseManager::saveAdmins(const vector<Admin>& admins) {
    saveObjects(admins, ADMINS_DB);
    cachedAdmins = admins;
}

bool DatabaseManager::addAdmin(const Admin& admin) {
    return addObject(cachedAdmins, admin, ADMINS_DB);
}

Admin* DatabaseManager::findAdminByEmail(const string& email) {
    return findObjectByKey<Admin, string>(cachedAdmins, email, &Admin::getEmail);
}

bool DatabaseManager::updateAdmin(const string& email, const Admin& updatedAdmin) {
    return updateObject<Admin, string>(cachedAdmins, email, updatedAdmin, ADMINS_DB, &Admin::getEmail);
}

bool DatabaseManager::deleteAdmin(const string& email) {
    return deleteObject<Admin, string>(cachedAdmins, email, ADMINS_DB, &Admin::getEmail);
}

// DiningAuthority operations
vector<DiningAuthority> DatabaseManager::loadDiningAuthorities() {
    return loadObjects<DiningAuthority>(DINING_AUTH_DB);
}

void DatabaseManager::saveDiningAuthorities(const vector<DiningAuthority>& authorities) {
    saveObjects(authorities, DINING_AUTH_DB);
    cachedDiningAuthorities = authorities;
}

bool DatabaseManager::addDiningAuthority(const DiningAuthority& authority) {
    return addObject(cachedDiningAuthorities, authority, DINING_AUTH_DB);
}

DiningAuthority* DatabaseManager::findDiningAuthorityByEmail(const string& email) {
    return findObjectByKey<DiningAuthority, string>(cachedDiningAuthorities, email, &DiningAuthority::getEmail);
}

bool DatabaseManager::updateDiningAuthority(const string& email, const DiningAuthority& updatedAuthority) {
    return updateObject<DiningAuthority, string>(cachedDiningAuthorities, email, updatedAuthority, DINING_AUTH_DB, &DiningAuthority::getEmail);
}

bool DatabaseManager::deleteDiningAuthority(const string& email) {
    return deleteObject<DiningAuthority, string>(cachedDiningAuthorities, email, DINING_AUTH_DB, &DiningAuthority::getEmail);
}

// Active Token operations
vector<MealToken> DatabaseManager::loadActiveTokens() {
    return loadObjects<MealToken>(ACTIVE_TOKENS_DB);
}

void DatabaseManager::saveActiveTokens(const vector<MealToken>& tokens) {
    saveObjects(tokens, ACTIVE_TOKENS_DB);
    cachedActiveTokens = tokens;
}

bool DatabaseManager::addActiveToken(const MealToken& token) {
    return addObject(cachedActiveTokens, token, ACTIVE_TOKENS_DB);
}

MealToken* DatabaseManager::findActiveTokenByID(const string& tokenID) {
    return findObjectByKey(cachedActiveTokens, tokenID, &MealToken::getTokenNumber);
}

bool DatabaseManager::updateActiveToken(const string& tokenID, const MealToken& updatedToken) {
    return updateObject(cachedActiveTokens, tokenID, updatedToken, ACTIVE_TOKENS_DB, &MealToken::getTokenNumber);
}

bool DatabaseManager::deleteActiveToken(const string& tokenID) {
    return deleteObject(cachedActiveTokens, tokenID, ACTIVE_TOKENS_DB, &MealToken::getTokenNumber);
}

// Used Token operations
vector<MealToken> DatabaseManager::loadUsedTokens() {
    return loadObjects<MealToken>(USED_TOKENS_DB);
}

void DatabaseManager::saveUsedTokens(const vector<MealToken>& tokens) {
    saveObjects(tokens, USED_TOKENS_DB);
    cachedUsedTokens = tokens;
}

bool DatabaseManager::addUsedToken(const MealToken& token) {
    return addObject(cachedUsedTokens, token, USED_TOKENS_DB);
}

MealToken* DatabaseManager::findUsedTokenByID(const string& tokenID) {
    return findObjectByKey(cachedUsedTokens, tokenID, &MealToken::getTokenNumber);
}

bool DatabaseManager::updateUsedToken(const string& tokenID, const MealToken& updatedToken) {
    return updateObject(cachedUsedTokens, tokenID, updatedToken, USED_TOKENS_DB, &MealToken::getTokenNumber);
}

bool DatabaseManager::deleteUsedToken(const string& tokenID) {
    return deleteObject(cachedUsedTokens, tokenID, USED_TOKENS_DB, &MealToken::getTokenNumber);
}

// Review operations
vector<MealReview> DatabaseManager::loadReviews() {
    return loadObjects<MealReview>(REVIEWS_DB);
}

void DatabaseManager::saveReviews(const vector<MealReview>& reviews) {
    saveObjects(reviews, REVIEWS_DB);
    cachedReviews = reviews;
}

bool DatabaseManager::addReview(const MealReview& review) {
    return addObject(cachedReviews, review, REVIEWS_DB);
}

MealReview* DatabaseManager::findReviewByID(const string& reviewID) {
    // MealReview uses tokenNumber as identifier, not reviewID
    return findObjectByKey<MealReview, string>(cachedReviews, reviewID, &MealReview::getTokenNumber);
}

bool DatabaseManager::updateReview(const string& reviewID, const MealReview& updatedReview) {
    // MealReview uses tokenNumber as identifier, not reviewID
    return updateObject<MealReview, string>(cachedReviews, reviewID, updatedReview, REVIEWS_DB, &MealReview::getTokenNumber);
}

bool DatabaseManager::deleteReview(const string& reviewID) {
    // MealReview uses tokenNumber as identifier, not reviewID
    return deleteObject<MealReview, string>(cachedReviews, reviewID, REVIEWS_DB, &MealReview::getTokenNumber);
}

// Meal operations
vector<Meal> DatabaseManager::loadMeals() {
    return loadObjects<Meal>(MEALS_DB);
}

void DatabaseManager::saveMeals(const vector<Meal>& meals) {
    saveObjects(meals, MEALS_DB);
    cachedMeals = meals;
}

bool DatabaseManager::addMeal(const Meal& meal) {
    return addObject(cachedMeals, meal, MEALS_DB);
}

bool DatabaseManager::updateMeal(const string& mealKey, const Meal& updatedMeal) {
    // Use a composite key: date_hall_mealType for unique identification
    for (size_t i = 0; i < cachedMeals.size(); i++) {
        string existingKey = cachedMeals[i].getDate() + "_" +
                           cachedMeals[i].getHallName() + "_" +
                           to_string(static_cast<int>(cachedMeals[i].getMealType()));
        if (existingKey == mealKey) {
            cachedMeals[i] = updatedMeal;
            saveObjects(cachedMeals, MEALS_DB);
            return true;
        }
    }
    return false;
}

bool DatabaseManager::deleteMeal(const string& mealKey) {
    // Use a composite key: date_hall_mealType for unique identification
    for (auto it = cachedMeals.begin(); it != cachedMeals.end(); it++) {
        string existingKey = it->getDate() + "_" +
                           it->getHallName() + "_" +
                           to_string(static_cast<int>(it->getMealType()));
        if (existingKey == mealKey) {
            cachedMeals.erase(it);
            saveObjects(cachedMeals, MEALS_DB);
            return true;
        }
    }
    return false;
}

vector<Meal> DatabaseManager::getMealsByHall(const string& hall) {
    vector<Meal> result;
    for (const auto& meal : cachedMeals) {
        if (meal.getHallName() == hall) {
            result.push_back(meal);
        }
    }
    return result;
}

vector<Meal> DatabaseManager::getMealsByDate(const string& date) {
    vector<Meal> result;
    for (const auto& meal : cachedMeals) {
        if (meal.getDate() == date) {
            result.push_back(meal);
        }
    }
    return result;
}

vector<Meal> DatabaseManager::getMealsByType(MealType type) {
    vector<Meal> result;
    for (const auto& meal : cachedMeals) {
        if (meal.getMealType() == type) {
            result.push_back(meal);
        }
    }
    return result;
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
    cachedActiveTokens.clear();
    cachedUsedTokens.clear();
    cachedReviews.clear();
    cachedMeals.clear();

    saveStudents(cachedStudents);
    saveTeachers(cachedTeachers);
    saveAdmins(cachedAdmins);
    saveDiningAuthorities(cachedDiningAuthorities);
    saveActiveTokens(cachedActiveTokens);
    saveUsedTokens(cachedUsedTokens);
    saveReviews(cachedReviews);
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

size_t DatabaseManager::getSystemAdminCount() {
    size_t count = 0;
    for (const auto& admin : cachedAdmins) {
        if (admin.getAdminType() == AdminType::SystemAdmin) {
            count++;
        }
    }
    return count;
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
    cout << "Active Tokens: " << cachedActiveTokens.size() << endl;
    cout << "Used Tokens: " << cachedUsedTokens.size() << endl;
    cout << "Reviews: " << cachedReviews.size() << endl;
    cout << "Total Users: " << (getStudentCount() + getTeacherCount() + getAdminCount() + getDiningAuthorityCount()) << endl;
}