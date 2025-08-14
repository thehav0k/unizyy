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

using namespace std;

// Database file paths
const string MEALS_DB = "Database/meals.dat";
const string MEAL_REVIEWS_DB = "Database/meal_reviews.dat";
const string ACTIVE_TOKENS_DB = "Database/active_tokens.dat";
const string USED_TOKENS_DB = "Database/used_tokens.dat";

// TokenManager static member
const string TokenManager::TOKEN_FOLDER = "Meal Tokens/";

// =============================================================================
// MEAL CLASS IMPLEMENTATION
// =============================================================================

// Constructors
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

// Operations
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
    cout << "Price: $" << fixed << setprecision(2) << price << endl;
    cout << "Available Quantity: " << availableQuantity << endl;
    cout << "Date: " << date << endl;
    cout << "Time: " << time << endl;
    cout << "Hall: " << getHallName() << endl;
    cout << "Status: " << (isAvailable ? "Available" : "Not Available") << endl;
}

// Static utility functions
string Meal::mealTypeToString(MealType type) {
    switch (type) {
        case MealType::BREAKFAST: return "Breakfast";
        case MealType::LUNCH: return "Lunch";
        case MealType::DINNER: return "Dinner";
        default: return "Unknown";
    }
}

MealType Meal::stringToMealType(const string& typeStr) {
    if (typeStr == "Breakfast") return MealType::BREAKFAST;
    if (typeStr == "Lunch") return MealType::LUNCH;
    if (typeStr == "Dinner") return MealType::DINNER;
    return MealType::LUNCH; // default
}

// =============================================================================
// MEAL TOKEN CLASS IMPLEMENTATION
// =============================================================================

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

// Operations
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

string MealToken::generateTokenNumber() {
    return "TKN" + to_string(time(nullptr)) + to_string(rand() % 1000);
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

// =============================================================================
// MEAL REVIEW CLASS IMPLEMENTATION
// =============================================================================

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

// =============================================================================
// TOKEN MANAGER CLASS IMPLEMENTATION
// =============================================================================

TokenManager::TokenManager() {
    createTokenFolder();
    loadAllTokens();
}

string TokenManager::buyToken(const string& studentEmail, const string& hallName,
                             MealType mealType, const Meal& meal) {
    string tokenNumber = MealToken::generateTokenNumber();
    Date today;
    // Determine valid date from meal's date string if valid, else today
    Date parsedDate(meal.getDate());
    Date validDate = parsedDate.isValid() ? parsedDate : today;

    MealToken newToken(tokenNumber, studentEmail, meal.getMealName(),
                      mealType, hallName, meal.getPrice(), today, validDate);

    activeTokens.push_back(newToken);
    saveAllTokens();

    // Save individual token file
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
    // Check if student already has an active token for this meal type on this date
    for (const auto& token : activeTokens) {
        if (token.getMealType() == mealType &&
            token.getValidDate().toString() == forDate.toString()) {
            return false; // Already has token for this meal type today
        }
    }
    return true;
}

bool TokenManager::addReview(const string& studentEmail, const string& tokenNumber,
                            MealRating rating, const string& comment) {
    // Find the used token
    for (auto& token : usedTokens) {
        if (token.getTokenNumber() == tokenNumber && token.getStudentEmail() == studentEmail) {
            Date today;
            // Use token meal name if available (stored in token.getMealName())
            MealReview review(studentEmail, tokenNumber, token.getMealName().empty()?"Meal":token.getMealName(), rating, comment,
                              today.toString(), token.getHallName());
            reviews.push_back(review);
            token.markAsReviewed();
            saveAllTokens(); // persist reviewed status
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
    // NOTE: cannot modify inside const; caller should invoke cleanupExpiredTokens beforehand
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

void TokenManager::saveAllTokens() {
    filesystem::create_directories("Database");
    ofstream outActive(ACTIVE_TOKENS_DB, ios::binary | ios::trunc);
    for (const auto& t : activeTokens) outActive.write(reinterpret_cast<const char*>(&t), sizeof(MealToken));
    ofstream outUsed(USED_TOKENS_DB, ios::binary | ios::trunc);
    for (const auto& t : usedTokens) outUsed.write(reinterpret_cast<const char*>(&t), sizeof(MealToken));
}

void TokenManager::loadAllTokens() {
    activeTokens.clear();
    usedTokens.clear();
    auto loadFile = [&](const string& path, vector<MealToken>& target){
        ifstream in(path, ios::binary);
        if (!in.is_open()) return;
        in.seekg(0, ios::end);
        streampos size = in.tellg();
        if (size < 0) return; // unexpected
        if (size % static_cast<streampos>(sizeof(MealToken)) != 0) {
            in.close();
            // Incompatible old format -> truncate file
            ofstream trunc(path, ios::trunc);
            return;
        }
        in.seekg(0, ios::beg);
        MealToken temp;
        while (in.read(reinterpret_cast<char*>(&temp), sizeof(MealToken))) target.push_back(temp);
    };
    loadFile(ACTIVE_TOKENS_DB, activeTokens);
    loadFile(USED_TOKENS_DB, usedTokens);
}

// =============================================================================
// MEAL UTILS CLASS IMPLEMENTATION
// =============================================================================

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
            // 07:00 - 10:00
            return hour >= 7 && hour <= 10;
        case MealType::LUNCH:
            // 12:00 - 14:30 (approximate to hour 12-14)
            return hour >= 12 && hour <= 14;
        case MealType::DINNER:
            // 19:00 - 22:30 (approximate to hour 19-22)
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

vector<string> MealUtils::getAvailableHalls() {
    return {
        "Al_Beruni_Hall",
        "Kazi_Motahar_Hossain_Hall",
        "Shaheed_Rafiq_Jabbar_Hall",
        "Bangamata_Sheikh_Fazilatunnesa_Mujib_Hall",
        "Sheikh_Russell_Hall"
    };
}

void MealUtils::displayMealMenu(const vector<Meal>& meals) {
    cout << "\n=== Available Meals ===" << endl;
    if (meals.empty()) {
        cout << "No meals available." << endl;
        return;
    }

    for (size_t i = 0; i < meals.size(); ++i) {
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

// Additional implementations for MealToken methods
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

void MealReview::displayReview() const {
    cout << "Student: " << getStudentEmail() << endl;
    cout << "Token: " << getTokenNumber() << endl;
    cout << "Meal: " << getMealName() << endl;
    cout << "Rating: " << ratingToString(rating) << " (" << static_cast<int>(rating) << "/5)" << endl;
    cout << "Comment: " << getComment() << endl;
    cout << "Review Date: " << getReviewDate() << endl;
    cout << "Hall: " << getHallName() << endl;
}

// =============================================================================
// PERSISTENCE IMPLEMENTATION FOR MEAL
// =============================================================================

// Helper: load all meals from binary file
vector<Meal> Meal::loadAllMealsFromDatabase() {
    vector<Meal> meals;
    ifstream in(MEALS_DB, ios::binary);
    if (!in.is_open()) return meals; // no file yet
    Meal temp;
    while (in.read(reinterpret_cast<char*>(&temp), sizeof(Meal))) {
        meals.push_back(temp);
    }
    return meals;
}

size_t Meal::getMealCount() { return loadAllMealsFromDatabase().size(); }

void Meal::displayAllMeals() {
    auto meals = loadAllMealsFromDatabase();
    if (meals.empty()) {
        cout << "No meals in database." << endl;
        return;
    }
    for (const auto& m : meals) {
        m.displayMeal();
        cout << "---" << endl;
    }
}

void Meal::initializeMealDatabase() {
    // Ensure directory exists
    filesystem::create_directories("Database");
    // Create file if not present
    if (!filesystem::exists(MEALS_DB)) {
        ofstream out(MEALS_DB, ios::binary); // empty file
    }
}

// Filter helpers
vector<Meal> Meal::loadMealsByHall(const string& hall) {
    vector<Meal> out;
    for (const auto& m : loadAllMealsFromDatabase()) {
        if (m.getHallName() == hall) out.push_back(m);
    }
    return out;
}

vector<Meal> Meal::loadMealsByDate(const string& dateStr) {
    vector<Meal> out;
    for (const auto& m : loadAllMealsFromDatabase()) {
        if (m.getDate() == dateStr) out.push_back(m);
    }
    return out;
}

vector<Meal> Meal::loadMealsByType(MealType type) {
    vector<Meal> out;
    for (const auto& m : loadAllMealsFromDatabase()) {
        if (m.getMealType() == type) out.push_back(m);
    }
    return out;
}

bool Meal::deleteMealFromDatabase(const string& dateStr, const string& hallStr, MealType type) {
    auto meals = loadAllMealsFromDatabase();
    size_t before = meals.size();
    meals.erase(remove_if(meals.begin(), meals.end(), [&](const Meal& m){
        return m.getDate() == dateStr && m.getHallName() == hallStr && m.getMealType() == type;
    }), meals.end());
    if (meals.size() == before) return false; // nothing removed
    ofstream out(MEALS_DB, ios::binary | ios::trunc);
    for (const auto& m : meals) {
        out.write(reinterpret_cast<const char*>(&m), sizeof(Meal));
    }
    return true;
}

bool Meal::saveMealToDatabase() const {
    auto meals = loadAllMealsFromDatabase();
    bool updated = false;
    string thisName = getMealName();
    for (auto& m : meals) {
        if (m.getMealName() == thisName && m.getDate() == getDate() && m.getHallName() == getHallName() && m.getMealType() == getMealType()) {
            m = *this;
            updated = true;
            break;
        }
    }
    if (!updated) meals.push_back(*this);
    ofstream out(MEALS_DB, ios::binary | ios::trunc);
    if (!out.is_open()) return false;
    for (const auto& m : meals) {
        out.write(reinterpret_cast<const char*>(&m), sizeof(Meal));
    }
    return true;
}

// Binary helpers (object-level)
void Meal::writeToBinaryFile(ofstream& out) const { out.write(reinterpret_cast<const char*>(this), sizeof(Meal)); }
void Meal::readFromBinaryFile(ifstream& in) { in.read(reinterpret_cast<char*>(this), sizeof(Meal)); }

bool Meal::writeDirectlyToFile(const string& filename) const {
    ofstream out(filename, ios::binary | ios::app);
    if (!out.is_open()) return false;
    writeToBinaryFile(out);
    return true;
}

bool Meal::readDirectlyFromFile(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in.is_open()) return false;
    return static_cast<bool>(in.read(reinterpret_cast<char*>(this), sizeof(Meal)));
}

bool Meal::writeDirectlyToDatabase() { return false; }
vector<Meal> Meal::loadDirectlyFromDatabase() { return loadAllMealsFromDatabase(); }

// Expiration check (assumes date string is DD-MM-YYYY)
bool Meal::isExpired() const {
    Date mealDate(getDate());
    Date today = Date::getCurrentDate();
    return mealDate < today;
}

// =============================================================================
// END IMPLEMENTATIONS
// =============================================================================
