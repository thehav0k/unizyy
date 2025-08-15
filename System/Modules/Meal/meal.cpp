//
// Created by Md. Asif Khan on 11/8/25.
// Meal Management System Implementation
//

#include "meal.h"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include "../../../Core/Utils/StringHelper.h"
#include "../../../Core/Database/DatabaseManager.h"
#include <cstring>

using namespace std;

// Database gular file directory location
const string MEALS_DB = "Database/meals.dat";
const string MEAL_REVIEWS_DB = "Database/meal_reviews.dat";
const string ACTIVE_TOKENS_DB = "Database/active_tokens.dat";
const string USED_TOKENS_DB = "Database/used_tokens.dat";
const string TokenManager::TOKEN_FOLDER = "Meal Tokens/";

vector<Meal> Meal::cachedMeals; // cache akare sob load
bool Meal::mealsLoaded = false; // boolean flag rakhchi jate barbar load na kora lage

// read write sob hoice database manager class diye
void Meal::loadMealsIntoCache() {
    if (mealsLoaded) return;
    cachedMeals.clear();

    DatabaseManager::loadObjects(cachedMeals, "meals");
    mealsLoaded = true;
}
// basically vector to file e convert hoilo
void Meal::saveMealsToDisk() {
    DatabaseManager::saveObjects(cachedMeals, "meals");
}

void Meal::initializeMealDatabase() {
    if (!mealsLoaded) {
        loadMealsIntoCache();
    }
}

size_t Meal::getMealCount() { ensureMealsLoaded(); return cachedMeals.size(); }

vector<Meal> Meal::loadAllMealsFromDatabase() { ensureMealsLoaded(); return cachedMeals; }

vector<Meal> Meal::loadMealsByHall(const string& hall) {
    ensureMealsLoaded();
    vector<Meal> out;
    for (const auto &m : cachedMeals) if (m.getHallName() == hall) out.push_back(m);
    return out;
}

vector<Meal> Meal::loadMealsByDate(const string& dateStr) {
    ensureMealsLoaded();
    vector<Meal> out;
    for (const auto &m : cachedMeals) if (m.getDate() == dateStr) out.push_back(m);
    return out;
}

vector<Meal> Meal::loadMealsByType(MealType type) {
    ensureMealsLoaded();
    vector<Meal> out;
    for (const auto &m : cachedMeals) if (m.getMealType() == type) out.push_back(m);
    return out;
}

bool Meal::addMeal(const Meal &meal) {
    ensureMealsLoaded();
    for (auto &m : cachedMeals) {
        if (m.getDate() == meal.getDate() && m.getHallName() == meal.getHallName() && m.getMealType() == meal.getMealType()) {
            m = meal;
            saveMealsToDisk();
            return true;
        }
    }
    cachedMeals.push_back(meal);
    saveMealsToDisk();
    return true;
}

bool Meal::updateMeal(const string &dateStr, const string &hallName, MealType type, const Meal &updatedMeal) {
    ensureMealsLoaded();
    for (auto &m : cachedMeals) {
        if (m.getDate() == dateStr && m.getHallName() == hallName && m.getMealType() == type) {
            m = updatedMeal;
            saveMealsToDisk();
            return true;
        }
    }
    return false;
}

bool Meal::deleteMealFromDatabase(const string& dateStr, const string& hallStr, MealType type) {
    ensureMealsLoaded();
    size_t before = cachedMeals.size();
    cachedMeals.erase(remove_if(cachedMeals.begin(), cachedMeals.end(), [&](const Meal &m){
        return m.getDate() == dateStr && m.getHallName() == hallStr && m.getMealType() == type;
    }), cachedMeals.end());
    if (cachedMeals.size() == before) return false;
    saveMealsToDisk();
    return true;
}

void Meal::displayAllMeals() {
    ensureMealsLoaded();
    if (cachedMeals.empty()) {
        cout << "No meals in database." << endl;
        return;
    }
    for (const auto &m : cachedMeals) {
        m.displayMeal();
        cout << "---" << endl;
    }
}

bool Meal::saveMealToDatabase() const { return addMeal(*this); }

Meal::Meal(const string& name, const string& desc, MealType type, double mealPrice,
           int quantity, const string& dateStr, const string& mealTime, const string& hall)
    : mealType(type), price(mealPrice), availableQuantity(quantity),
      isAvailable(quantity > 0), hallName(stringToHalls(hall)) {

    string package = name + " - " + desc;
    StringHelper::safeStringToCharArray(package, MealPackage, "MealPackage");
    StringHelper::safeStringToCharArray(dateStr, this->date, "date");
    StringHelper::safeStringToCharArray(mealTime, this->time, "time");
}

Meal::Meal() : mealType(MealType::LUNCH), price(0.0), availableQuantity(0),
               isAvailable(false), hallName(Halls::Al_Beruni_Hall) {
    StringHelper::stringToCharArray("Default Meal Package", MealPackage);
    Date today;
    StringHelper::safeStringToCharArray(today.toString(), this->date, "date");
    StringHelper::stringToCharArray("00:00", time);
}

// Getters
string Meal::getMealName() const {
    string package(MealPackage);
    size_t pos = package.find(" - ");
    if (pos != string::npos) {
        return package.substr(0, pos);
    }
    return package;
}

string Meal::getDescription() const {
    string package(MealPackage);
    size_t pos = package.find(" - ");
    if (pos != string::npos) {
        return package.substr(pos + 3);
    }
    return "";
}

MealType Meal::getMealType() const { return mealType; }
double Meal::getPrice() const { return price; }
int Meal::getAvailableQuantity() const { return availableQuantity; }
bool Meal::getIsAvailable() const { return isAvailable; }
string Meal::getDate() const { return string(date); }
string Meal::getTime() const { return string(time); }string Meal::getHallName() const { return hallToString(hallName); }

// Setters
void Meal::setMealName(const string& name) {
    string desc = getDescription();
    string package = name + (desc.empty() ? "" : " - " + desc);
    StringHelper::safeStringToCharArray(package, MealPackage, "MealPackage");
}

void Meal::setDescription(const string& desc) {
    string name = getMealName();
    string package = name + " - " + desc;
    StringHelper::safeStringToCharArray(package, MealPackage, "MealPackage");
}

void Meal::setMealType(MealType type) { mealType = type; }
void Meal::setPrice(double mealPrice) { price = mealPrice; }
void Meal::setAvailableQuantity(int quantity) {
    availableQuantity = quantity;
    isAvailable = quantity > 0;
}
void Meal::setIsAvailable(bool available) { isAvailable = available; }
void Meal::setDate(const string& dateStr) {
    StringHelper::safeStringToCharArray(dateStr, date, "date");
}
void Meal::setTime(const string& timeStr) {
    StringHelper::safeStringToCharArray(timeStr, time, "time");
}
void Meal::setHallName(const string& hall) {
    hallName = stringToHalls(hall);
}

// Meal er sob functions
bool Meal::orderMeal(int quantity) {
    if (!isAvailable || availableQuantity < quantity) {
        return false;
    }
    availableQuantity -= quantity;
    if (availableQuantity <= 0) {
        isAvailable = false;
    }
    return true;
}

void Meal::displayMeal() const {
    cout << "Meal: " << getMealName() << endl;
    cout << "Description: " << getDescription() << endl;
    cout << "Type: " << mealTypeToString(mealType) << endl;
    cout << "Price: ৳" << fixed << setprecision(2) << price << " BDT" << endl;
    cout << "Available Quantity: " << availableQuantity << endl;
    cout << "Date: " << date << endl;
    cout << "Time: " << time << endl;
    cout << "Hall: " << getHallName() << endl;
    cout << "Status: " << (isAvailable ? "Available" : "Not Available") << endl;
}

// hlper enum to string
string Meal::mealTypeToString(MealType type) {
    switch (type) {
        case MealType::BREAKFAST: return "Breakfast";
        case MealType::LUNCH: return "Lunch";
        case MealType::DINNER: return "Dinner";
        default: return "Unknown";
    }
}
// ulta
MealType Meal::stringToMealType(const string& typeStr) {
    if (typeStr == "Breakfast") return MealType::BREAKFAST;
    if (typeStr == "Lunch") return MealType::LUNCH;
    if (typeStr == "Dinner") return MealType::DINNER;
    return MealType::LUNCH; // default
}
// token clss
// Default hall dewa Al Baruni
MealToken::MealToken() : mealType(MealType::LUNCH), hallName(Halls::Al_Beruni_Hall),
                         paidAmount(0.0), status(TokenStatus::ACTIVE) {
    StringHelper::stringToCharArray("", tokenNumber);
    StringHelper::stringToCharArray("", studentEmail);
    StringHelper::stringToCharArray("", mealName);
    StringHelper::stringToCharArray("00:00", purchaseTime);
}

MealToken::MealToken(const string& tokenNum, const string& studentEmailStr,
                     const string& mealNameStr, MealType type, const string& hall,
                     double amount, const Date& purchaseDate, const Date& validDate)
    : mealType(type), hallName(stringToHalls(hall)), paidAmount(amount),
      purchaseDate(purchaseDate), validDate(validDate), status(TokenStatus::ACTIVE) {

    StringHelper::safeStringToCharArray(tokenNum, tokenNumber, "tokenNumber");
    StringHelper::safeStringToCharArray(studentEmailStr, studentEmail, "studentEmail");
    StringHelper::safeStringToCharArray(mealNameStr, mealName, "mealName");
    StringHelper::stringToCharArray(MealUtils::getCurrentTime(), purchaseTime);
}

// Getters
string MealToken::getTokenNumber() const { return string(tokenNumber); }
string MealToken::getStudentEmail() const { return string(studentEmail); }
string MealToken::getMealName() const { return string(mealName); }
MealType MealToken::getMealType() const { return mealType; }
string MealToken::getHallName() const { return hallToString(hallName); }
double MealToken::getPaidAmount() const { return paidAmount; }
Date MealToken::getPurchaseDate() const { return purchaseDate; }
Date MealToken::getValidDate() const { return validDate; }
TokenStatus MealToken::getStatus() const { return status; }
string MealToken::getPurchaseTime() const { return string(purchaseTime); }

// Meal token er kaj gula
void MealToken::setStatus(TokenStatus newStatus) { status = newStatus; }

bool MealToken::isValid() const {
    return status == TokenStatus::ACTIVE && !isExpired();
}

bool MealToken::canBeUsed() const {
    if (!isValid()) return false;
    Date today = Date::getCurrentDate();
    if (!(validDate == today)) return false;
    return MealUtils::isWithinMealTime(mealType);
}

bool MealToken::isExpired() const {
    Date today = Date::getCurrentDate();
    return validDate < today;
}

void MealToken::markAsUsed() { status = TokenStatus::USED; }
void MealToken::markAsReviewed() { status = TokenStatus::REVIEWED; }

// token number generate
// TKN er sathe timestamp jog
// er sathe 3 digit er random number
string MealToken::generateTokenNumber() {
    return "TKN" + to_string(time(nullptr)) + to_string(rand() % 899 + 100);
}

string MealToken::tokenStatusToString(TokenStatus st) {
    switch (st) {
        case TokenStatus::ACTIVE: return "Active";
        case TokenStatus::USED: return "Used";
        case TokenStatus::EXPIRED: return "Expired";
        case TokenStatus::REVIEWED: return "Reviewed";
        default: return "Unknown";
    }
}

// Constructors
MealReview::MealReview() : rating(MealRating::GOOD), hallName(Halls::Al_Beruni_Hall),
                           batch(0), department(department::Department_of_Computer_Science_and_Engineering) {
    StringHelper::stringToCharArray("", Name);
    StringHelper::stringToCharArray("", tokenNumber);
    StringHelper::stringToCharArray("", mealName);
    StringHelper::stringToCharArray("", comment);
}

MealReview::MealReview(const string& email, const string& tokenNum, const string& meal,
                       MealRating mealRating, const string& commentStr, const string& date, const string& hall)
    : rating(mealRating), hallName(stringToHalls(hall)), batch(0), department(department::Department_of_Computer_Science_and_Engineering) {

    StringHelper::safeStringToCharArray(email, Name, "Name");
    StringHelper::safeStringToCharArray(tokenNum, tokenNumber, "tokenNumber");
    StringHelper::safeStringToCharArray(meal, mealName, "mealName");
    StringHelper::safeStringToCharArray(commentStr, comment, "comment");
    reviewDate = Date(date);
}

// Getters
string MealReview::getStudentEmail() const { return string(Name); }
string MealReview::getTokenNumber() const { return string(tokenNumber); }
string MealReview::getMealName() const { return string(mealName); }
MealRating MealReview::getRating() const { return rating; }
string MealReview::getComment() const { return string(comment); }
string MealReview::getReviewDate() const { return reviewDate.toString(); }
string MealReview::getHallName() const { return hallToString(hallName); }

string MealReview::ratingToString(MealRating rating) {
    switch (rating) {
        case MealRating::POOR: return "Poor";
        case MealRating::FAIR: return "Fair";
        case MealRating::GOOD: return "Good";
        case MealRating::VERY_GOOD: return "Very Good";
        case MealRating::EXCELLENT: return "Excellent";
        default: return "Unknown";
    }
}

void MealReview::displayReview() const {
    cout << "Student: " << getStudentEmail() << endl;
    cout << "Token: " << getTokenNumber() << endl;
    cout << "Meal: " << getMealName() << endl;
    cout << "Rating: " << ratingToString(rating) << endl;
    cout << "Comment: " << getComment() << endl;
    cout << "Date: " << getReviewDate() << endl;
    cout << "Hall: " << getHallName() << endl;
}

// Token manager class
TokenManager::TokenManager() {
    createTokenFolder();
    loadAllTokens();
    loadAllReviews();
}

string TokenManager::buyToken(const string& studentEmail, const string& hallName,
                             MealType mealType, const Meal& meal) {
    string tokenNumber = MealToken::generateTokenNumber();
    Date today;
    // Date thik ache nki check
    Date parsedDate(meal.getDate());
    Date validDate = parsedDate.isValid() ? parsedDate : today;

    MealToken newToken(tokenNumber, studentEmail, meal.getMealName(),
                      mealType, hallName, meal.getPrice(), today, validDate);

    activeTokens.push_back(newToken);
    saveAllTokens();
    newToken.saveToFile(TOKEN_FOLDER);
    return tokenNumber;
}

bool TokenManager::useToken(const string& tokenNumber, const string& studentEmail) {
    for (auto it = activeTokens.begin(); it != activeTokens.end(); ++it) {
        if (it->getTokenNumber() == tokenNumber && it->getStudentEmail() == studentEmail) {
            if (!it->canBeUsed()) return false;
            it->markAsUsed();
            usedTokens.push_back(*it);
            activeTokens.erase(it);
            saveAllTokens();
            return true;
        }
    }
    return false;
}

bool TokenManager::canBuyToken(const string& studentEmail, MealType mealType, const Date& forDate) const {
    // Token thakle r kinte dewa jbena
    for (const auto& token : activeTokens) {
        if (token.getMealType() == mealType &&
            token.getValidDate().toString() == forDate.toString()) {
            return false;
        }
    }
    return true;
}

bool TokenManager::addReview(const string& studentEmail, const string& tokenNumber,
                            MealRating rating, const string& comment) {
    for (auto& token : usedTokens) {
        if (token.getTokenNumber() == tokenNumber && token.getStudentEmail() == studentEmail) {
            Date today;
            MealReview review(studentEmail, tokenNumber, token.getMealName().empty()?"Meal":token.getMealName(), rating, comment,
                              today.toString(), token.getHallName());
            reviews.push_back(review);
            token.markAsReviewed();
// datagula save
            saveAllTokens();
            saveAllReviews();
            return true;
        }
    }
    return false;
}

void TokenManager::cleanupExpiredTokens() {
    bool changed = false;
    for (auto &t : activeTokens) {
        if (t.getStatus() == TokenStatus::ACTIVE && t.isExpired()) {
            t.setStatus(TokenStatus::EXPIRED);
            changed = true;
        }
    }
    if (changed) saveAllTokens();
}

vector<MealToken> TokenManager::getStudentTokens(const string& studentEmail) const {
    vector<MealToken> result;
    for (const auto& t : activeTokens) if (t.getStudentEmail() == studentEmail) result.push_back(t);
    for (const auto& t : usedTokens) if (t.getStudentEmail() == studentEmail) result.push_back(t);
    return result;
}

void TokenManager::displayStudentTokens(const string& studentEmail) const {
    cout << "\n=== Active Tokens ===" << endl;
    bool hasActive = false;
    for (const auto& token : activeTokens) {
        if (token.getStudentEmail() != studentEmail) continue;
        token.displayToken();
        hasActive = true;
        cout << "---" << endl;
    }
    if (!hasActive) cout << "No active tokens found." << endl;
    cout << "\n=== Used Tokens ===" << endl;
    bool hasUsed = false;
    for (const auto& token : usedTokens) {
        if (token.getStudentEmail() != studentEmail) continue;
        token.displayToken();
        hasUsed = true;
        cout << "---" << endl;
    }
    if (!hasUsed) cout << "No used tokens found." << endl;
}

void TokenManager::createTokenFolder() {
    filesystem::create_directories(TOKEN_FOLDER);
}

// Database Manager class diya vector
void TokenManager::saveAllTokens() {
    DatabaseManager::saveObjects(activeTokens, "active_tokens");
    DatabaseManager::saveObjects(usedTokens, "used_tokens");
}

void TokenManager::loadAllTokens() {
    activeTokens.clear();
    usedTokens.clear();
    DatabaseManager::loadObjects(activeTokens, "active_tokens");
    DatabaseManager::loadObjects(usedTokens, "used_tokens");
}

void TokenManager::saveAllReviews() {
    DatabaseManager::saveObjects(reviews, "meal_reviews");
}

void TokenManager::loadAllReviews() {
    reviews.clear();
    DatabaseManager::loadObjects(reviews, "meal_reviews");
}
// helper method gula
bool MealUtils::isWithinMealTime(MealType type) {
    int hour;
    if (Date::isSimulationActive() && Date::getSimulatedHour() >= 0) {
        hour = Date::getSimulatedHour();
    } else {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        hour = ltm->tm_hour;
    }
    switch (type) {
        case MealType::BREAKFAST:
            return hour >= 7 && hour <= 10;
        case MealType::LUNCH:
            return hour >= 12 && hour <= 14;
        case MealType::DINNER:
            return hour >= 19 && hour <= 22;
        default:
            return false;
    }
}

string MealUtils::getCurrentTime() {
    int hour, minute;
    if (Date::isSimulationActive() && Date::getSimulatedHour() >= 0) {
        hour = Date::getSimulatedHour();
        minute = 0;
    } else {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        hour = ltm->tm_hour;
        minute = ltm->tm_min;
    }
    stringstream ss;
    ss << setfill('0') << setw(2) << hour << ":" << setfill('0') << setw(2) << minute;
    return ss.str();
}

bool MealUtils::isMealTimeExpired(MealType type, const Date& mealDate) {
    Date today = Date::getCurrentDate();
    if (mealDate < today) return true;
    if (mealDate > today) return false;
    return !isWithinMealTime(type);
}
// Halls enums lagate hbe
vector<string> MealUtils::getAvailableHalls() {
    return {
        "Al Beruni Hall",
        "Meer Mosharraf Hossain Hall",
        "Shaheed Salam Barkat Hall",
        "AFM Kamaluddin Hall",
        "Moulana Bhasani Hall",
        "Bangabondhu Sheikh Majibur Rahman Hall",
        "Jatiya Kabi Kazi Nazrul Islam Hall",
        "Rabindra Nath Tagore Hall",
        "Shahid Tajuddin Ahmed Hall",
        "Shahid Sheikh Russel Hall",
        "Shaheed Rafiq Jabbar Hall",
        "Nawab Faizunnesa Hall",
        "Fazilatunnesa Hall",
        "Jahanara Imam Hall",
        "Preetilata Hall",
        "Begum Khaleda Zia Hall",
        "Sheikh Hasina Hall",
        "Bir Pratik Taramon Bibi Hall"
    };
}

void MealUtils::displayMealMenu(const vector<Meal>& meals) {
    cout << "\n=== Available Meals ===" << endl;
    if (meals.empty()) {
        cout << "No meals available." << endl;
        return;
    }

    for (size_t i = 0; i < meals.size(); i++) {
        cout << "Meal " << (i + 1) << ":" << endl;
        meals[i].displayMeal();
        cout << "---" << endl;
    }
}

void MealUtils::displayTokenReceipt(const MealToken& token) {
    cout << "\n=== MEAL TOKEN RECEIPT ===" << endl;
    cout << "Token Number: " << token.getTokenNumber() << endl;
    cout << "Meal Type: " << Meal::mealTypeToString(token.getMealType()) << endl;
    cout << "Hall: " << token.getHallName() << endl;
    cout << "Amount Paid: $" << fixed << setprecision(2) << token.getPaidAmount() << endl;
    cout << "Purchase Date: " << token.getPurchaseDate().toString() << endl;
    cout << "Valid Date: " << token.getValidDate().toString() << endl;
    cout << "Purchase Time: " << token.getPurchaseTime() << endl;
    TokenStatus effective = token.getStatus();
    if (effective == TokenStatus::ACTIVE && token.isExpired()) effective = TokenStatus::EXPIRED;
    cout << "Status: " << MealToken::tokenStatusToString(effective) << endl;
    cout << "=========================" << endl;
}

void MealToken::displayToken() const {
    cout << "Token: " << getTokenNumber() << endl;
    cout << "Type: " << Meal::mealTypeToString(mealType) << endl;
    cout << "Hall: " << getHallName() << endl;
    cout << "Amount: $" << fixed << setprecision(2) << paidAmount << endl;
    cout << "Valid Date: " << validDate.toString() << endl;
    TokenStatus effective = status;
    if (status == TokenStatus::ACTIVE && isExpired()) effective = TokenStatus::EXPIRED; // safety
    cout << "Status: " << MealToken::tokenStatusToString(effective) << endl;
}

void MealToken::saveToFile(const string& folderPath) const {
    string filename = folderPath + getTokenNumber() + ".txt";
    ofstream file(filename);
    if (file.is_open()) {
        file << "Token Number: " << getTokenNumber() << endl;
        file << "Meal Type: " << Meal::mealTypeToString(mealType) << endl;
        file << "Hall: " << getHallName() << endl;
        file << "Amount Paid: " << paidAmount << endl;
        file << "Purchase Date: " << purchaseDate.toString() << endl;
        file << "Valid Date: " << validDate.toString() << endl;
        file << "Purchase Time: " << purchaseTime << endl;
        file.close();
    }
}

vector<MealReview> TokenManager::getMealReviews(const string& mealName) const {
    vector<MealReview> result;
    for (const auto &r : reviews) if (r.getMealName() == mealName) result.push_back(r);
    return result;
}

vector<MealReview> TokenManager::getHallReviews(const string& hallName) const {
    vector<MealReview> result;
    for (const auto &r : reviews) if (r.getHallName() == hallName) result.push_back(r);
    return result;
}

void TokenManager::displayAllReviews() const {
    if (reviews.empty()) { cout << "No reviews available." << endl; return; }
    cout << "\n=== ALL MEAL REVIEWS ===" << endl;
    for (const auto &r : reviews) { r.displayReview(); cout << "---" << endl; }
}

bool TokenManager::hasReviewedToken(const string& tokenNumber) const {
    for (const auto &r : reviews) if (r.getTokenNumber() == tokenNumber) return true;
    return false;
}