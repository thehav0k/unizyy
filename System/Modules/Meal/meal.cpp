#include "meal.h"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <random>
#include "../../../Core/Utils/StringHelper.h"
#include "../../../Core/Database/DatabaseManager.h"

using namespace std;

//Constant folder path for meal token files
const string TokenManager::TOKEN_FOLDER = "Meal Tokens/";

//Static member definitions
vector<Meal> Meal::cachedMeals;
bool Meal::mealsLoaded = false;

// ========== HELPER FUNCTIONS ==========

// Meal er unique key banay (date + hall + type)
string generateMealKey(const string& dateStr, const string& hallName, MealType type) {
    return dateStr + "_" + hallName + "_" + to_string(static_cast<int>(type));
}

//Package string ke name & desc e divide kore
pair<string, string> parsePackageString(const string& package) {
    size_t pos = package.find(" - ");
    if (pos != string::npos) {
        return {package.substr(0, pos), package.substr(pos + 3)};
    }
    return {package, ""};
}

//Name & description diye package string create kore
string buildPackageString(const string& name, const string& description) {
    return description.empty() ? name : name + " - " + description;
}


template<size_t N>
void setCharArrayField(char (&field)[N], const string& value, const string& fieldName) {
    StringHelper::safeStringToCharArray(value, field, fieldName);
}

//Enum value ke string e convert kore
template<typename EnumType>
string enumToStringHelper(EnumType value, const vector<pair<EnumType, string>>& mapping, const string& defaultValue = "Unknown") {
    for (const auto& pair : mapping) {
        if (pair.first == value) return pair.second;
    }
    return defaultValue;
}


template<typename T, typename Predicate>
vector<T> filterCollection(const vector<T>& collection, Predicate pred) {
    vector<T> result;
    for (const auto& item : collection) {
        if (pred(item)) {
            result.push_back(item);
        }
    }
    return result;
}

// ========== MEAL CLASS IMPLEMENTATION ==========

// Static methods using DatabaseManager
void Meal::initializeMealDatabase() {
    DatabaseManager::initializeDatabase();
}

void Meal::ensureMealsLoaded() {
    if (!mealsLoaded) {
        cachedMeals = DatabaseManager::loadMeals();
        mealsLoaded = true;
    }
}

const vector<Meal>& Meal::getCachedMeals() {
    ensureMealsLoaded();
    return cachedMeals;
}

size_t Meal::getMealCount() {
    ensureMealsLoaded();
    return cachedMeals.size();
}

vector<Meal> Meal::loadAllMealsFromDatabase() {
    ensureMealsLoaded();
    return cachedMeals;
}

vector<Meal> Meal::loadMealsByHall(const string& hall) {
    return DatabaseManager::getMealsByHall(hall);
}

vector<Meal> Meal::loadMealsByDate(const string& dateStr) {
    return DatabaseManager::getMealsByDate(dateStr);
}

vector<Meal> Meal::loadMealsByType(MealType type) {
    return DatabaseManager::getMealsByType(type);
}

bool Meal::addMeal(const Meal &meal) {
    if (DatabaseManager::addMeal(meal)) {
        cachedMeals.push_back(meal);
        return true;
    }
    return false;
}

bool Meal::updateMeal(const string &dateStr, const string &hallName, MealType type, const Meal &updatedMeal) {
    string mealKey = generateMealKey(dateStr, hallName, type);
    if (DatabaseManager::updateMeal(mealKey, updatedMeal)) {
        // Update cache
        for (auto& meal : cachedMeals) {
            if (meal.getDate() == dateStr && meal.getHallName() == hallName && meal.getMealType() == type) {
                meal = updatedMeal;
                break;
            }
        }
        return true;
    }
    return false;
}

bool Meal::deleteMealFromDatabase(const string& dateStr, const string& hallStr, MealType type) {
    string mealKey = generateMealKey(dateStr, hallStr, type);
    if (DatabaseManager::deleteMeal(mealKey)) {
        // Update cache
        cachedMeals.erase(
            remove_if(cachedMeals.begin(), cachedMeals.end(),
                [&](const Meal& meal) {
                    return meal.getDate() == dateStr &&
                           meal.getHallName() == hallStr &&
                           meal.getMealType() == type;
                }),
            cachedMeals.end());
        return true;
    }
    return false;
}

void Meal::displayAllMeals() {
    ensureMealsLoaded();
    if (cachedMeals.empty()) {
        cout << "No meals in database." << endl;
        return;
    }
    for (const auto &meal : cachedMeals) {
        meal.displayMeal();
        cout << "---" << endl;
    }
}

bool Meal::saveMealToDatabase() const {
    return DatabaseManager::addMeal(*this);
}

// Constructors
Meal::Meal(const string& name, const string& desc, MealType type, double mealPrice,
           int quantity, const string& dateStr, const string& mealTime, const string& hall)
    : mealType(type), price(mealPrice), availableQuantity(quantity),
      isAvailable(quantity > 0), hallName(stringToHalls(hall)) {

    // Initialize char arrays with zeros first
    memset(MealPackage, 0, sizeof(MealPackage));
    memset(date, 0, sizeof(date));
    memset(time, 0, sizeof(time));

    string package = buildPackageString(name, desc);
    setCharArrayField(MealPackage, package, "MealPackage");
    setCharArrayField(this->date, dateStr, "date");
    setCharArrayField(this->time, mealTime, "time");
}

Meal::Meal() : mealType(MealType::LUNCH), price(0.0), availableQuantity(0),
               isAvailable(false), hallName(Halls::Al_Beruni_Hall) {
    // Initialize char arrays with zeros first
    memset(MealPackage, 0, sizeof(MealPackage));
    memset(date, 0, sizeof(date));
    memset(time, 0, sizeof(time));

    setCharArrayField(MealPackage, "Default Meal Package", "MealPackage");
    Date today;
    setCharArrayField(this->date, today.toString(), "date");
    setCharArrayField(this->time, "00:00", "time");
}

// Getters using helper function
string Meal::getMealName() const {
    auto [name, description] = parsePackageString(string(MealPackage));
    return name;
}

string Meal::getDescription() const {
    auto [name, description] = parsePackageString(string(MealPackage));
    return description;
}

// Simple getters
MealType Meal::getMealType() const { return mealType; }
double Meal::getPrice() const { return price; }
int Meal::getAvailableQuantity() const { return availableQuantity; }
bool Meal::getIsAvailable() const { return isAvailable; }
string Meal::getDate() const { return StringHelper::charArrayToString(date); }
string Meal::getTime() const { return StringHelper::charArrayToString(time); }
string Meal::getHallName() const { return hallToString(hallName); }

// Setters using helper functions
void Meal::setMealName(const string& name) {
    string desc = getDescription();
    string package = buildPackageString(name, desc);
    setCharArrayField(MealPackage, package, "MealPackage");
}

void Meal::setDescription(const string& desc) {
    string name = getMealName();
    string package = buildPackageString(name, desc);
    setCharArrayField(MealPackage, package, "MealPackage");
}

void Meal::setMealType(MealType type) { mealType = type; }
void Meal::setPrice(double mealPrice) { price = mealPrice; }

void Meal::setAvailableQuantity(int quantity) {
    availableQuantity = quantity;
    isAvailable = quantity > 0;
}

void Meal::setIsAvailable(bool available) { isAvailable = available; }

void Meal::setDate(const string& dateStr) {
    setCharArrayField(date, dateStr, "date");
}

void Meal::setTime(const string& timeStr) {
    setCharArrayField(time, timeStr, "time");
}

void Meal::setHallName(const string& hall) {
    hallName = stringToHalls(hall);
}

// Meal operations
bool Meal::orderMeal(int quantity) {
    if (!isAvailable || availableQuantity < quantity) {
        return false;
    }
    availableQuantity -= quantity;
    isAvailable = availableQuantity > 0;
    return true;
}

bool Meal::isExpired() const {
    Date mealDate(getDate());
    Date today = Date::getCurrentDate();
    return mealDate < today;
}

void Meal::displayMeal() const {
    cout << "Meal: " << getMealName() << endl;
    cout << "Description: " << getDescription() << endl;
    cout << "Type: " << mealTypeToString(mealType) << endl;
    cout << "Price: ৳" << fixed << setprecision(2) << price << " BDT" << endl;
    cout << "Available Quantity: " << availableQuantity << endl;
    cout << "Date: " << getDate() << endl;
    cout << "Time: " << getTime() << endl;
    cout << "Hall: " << getHallName() << endl;
    cout << "Status: " << (isAvailable ? "Available" : "Not Available") << endl;
}

// Enum conversion functions using DRY helper
string Meal::mealTypeToString(MealType type) {
    static const vector<pair<MealType, string>> mealTypeMap = {
        {MealType::BREAKFAST, "Breakfast"},
        {MealType::LUNCH, "Lunch"},
        {MealType::DINNER, "Dinner"}
    };
    return enumToStringHelper(type, mealTypeMap);
}

MealType Meal::stringToMealType(const string& typeStr) {
    if (typeStr == "Breakfast") return MealType::BREAKFAST;
    if (typeStr == "Lunch") return MealType::LUNCH;
    if (typeStr == "Dinner") return MealType::DINNER;
    return MealType::LUNCH; // default
}

// Regular meal functions - always available
Meal Meal::getRegularMeal(MealType type, const string& hallName, const string& date) {
    string name, description;
    double price;
    string time;

    switch (type) {
        case MealType::BREAKFAST:
            name = "Regular Breakfast";
            description = "Paratha, Egg, Vegetables, Tea";
            price = 50.0;
            time = "08:00";
            break;
        case MealType::LUNCH:
            name = "Regular Lunch";
            description = "Rice, Dal, Fish Curry, Vegetables";
            price = 80.0;
            time = "13:00";
            break;
        case MealType::DINNER:
            name = "Regular Dinner";
            description = "Rice, Meat Curry, Dal, Vegetables";
            price = 85.0;
            time = "20:00";
            break;
    }

    // Regular meals always have unlimited quantity
    return Meal(name, description, type, price, 999, date, time, hallName);
}

bool Meal::isRegularMealAvailable(MealType type, const string& hallName, const string& date) {
    // Regular meals are always available for any hall and any future date
    return true;
}

// ========== MEAL TOKEN CLASS IMPLEMENTATION ==========

MealToken::MealToken() : mealType(MealType::LUNCH), hallName(Halls::Al_Beruni_Hall),
                         paidAmount(0.0), status(TokenStatus::ACTIVE) {
    // Initialize char arrays with zeros first
    memset(tokenNumber, 0, sizeof(tokenNumber));
    memset(studentEmail, 0, sizeof(studentEmail));
    memset(mealName, 0, sizeof(mealName));
    memset(purchaseTime, 0, sizeof(purchaseTime));

    setCharArrayField(tokenNumber, "", "tokenNumber");
    setCharArrayField(studentEmail, "", "studentEmail");
    setCharArrayField(mealName, "", "mealName");
    setCharArrayField(purchaseTime, "00:00", "purchaseTime");
}

MealToken::MealToken(const string& tokenNum, const string& studentEmailStr,
                     const string& mealNameStr, MealType type, const string& hall,
                     double amount, const Date& purchaseDate, const Date& validDate)
    : mealType(type), hallName(stringToHalls(hall)), paidAmount(amount),
      purchaseDate(purchaseDate), validDate(validDate), status(TokenStatus::ACTIVE) {

    // Initialize char arrays with zeros first
    memset(tokenNumber, 0, sizeof(tokenNumber));
    memset(studentEmail, 0, sizeof(studentEmail));
    memset(mealName, 0, sizeof(mealName));
    memset(purchaseTime, 0, sizeof(purchaseTime));

    setCharArrayField(tokenNumber, tokenNum, "tokenNumber");
    setCharArrayField(studentEmail, studentEmailStr, "studentEmail");
    setCharArrayField(mealName, mealNameStr, "mealName");
    setCharArrayField(purchaseTime, MealUtils::getCurrentTime(), "purchaseTime");
}

// Getters using StringHelper
string MealToken::getTokenNumber() const { return StringHelper::charArrayToString(tokenNumber); }
string MealToken::getStudentEmail() const { return StringHelper::charArrayToString(studentEmail); }
string MealToken::getMealName() const { return StringHelper::charArrayToString(mealName); }
MealType MealToken::getMealType() const { return mealType; }
string MealToken::getHallName() const { return hallToString(hallName); }
double MealToken::getPaidAmount() const { return paidAmount; }
Date MealToken::getPurchaseDate() const { return purchaseDate; }
Date MealToken::getValidDate() const { return validDate; }
TokenStatus MealToken::getStatus() const { return status; }
string MealToken::getPurchaseTime() const { return StringHelper::charArrayToString(purchaseTime); }

// Token operations
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
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(100, 999);
    return "TKN" + to_string(time(nullptr)) + to_string(dis(gen));
}

string MealToken::tokenStatusToString(TokenStatus st) {
    static const vector<pair<TokenStatus, string>> statusMap = {
        {TokenStatus::ACTIVE, "Active"},
        {TokenStatus::USED, "Used"},
        {TokenStatus::EXPIRED, "Expired"},
        {TokenStatus::REVIEWED, "Reviewed"}
    };
    return enumToStringHelper(st, statusMap);
}

void MealToken::displayToken() const {
    cout << "Token: " << getTokenNumber() << endl;
    cout << "Type: " << Meal::mealTypeToString(mealType) << endl;
    cout << "Hall: " << getHallName() << endl;
    cout << "Amount: $" << fixed << setprecision(2) << paidAmount << endl;
    cout << "Valid Date: " << validDate.toString() << endl;
    TokenStatus effective = status;
    if (status == TokenStatus::ACTIVE && isExpired()) effective = TokenStatus::EXPIRED;
    cout << "Status: " << tokenStatusToString(effective) << endl;
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
        file << "Purchase Time: " << getPurchaseTime() << endl;
        file.close();
    }
}

// ========== MEAL REVIEW CLASS IMPLEMENTATION ==========

MealReview::MealReview() : rating(MealRating::GOOD), hallName(Halls::Al_Beruni_Hall),
                           batch(0), department(department::Department_of_Computer_Science_and_Engineering) {
    // Initialize char arrays with zeros first
    memset(Name, 0, sizeof(Name));
    memset(tokenNumber, 0, sizeof(tokenNumber));
    memset(mealName, 0, sizeof(mealName));
    memset(comment, 0, sizeof(comment));

    setCharArrayField(Name, "", "Name");
    setCharArrayField(tokenNumber, "", "tokenNumber");
    setCharArrayField(mealName, "", "mealName");
    setCharArrayField(comment, "", "comment");
}

MealReview::MealReview(const string& email, const string& tokenNum, const string& meal,
                       MealRating mealRating, const string& commentStr, const string& date, const string& hall)
    : rating(mealRating), hallName(stringToHalls(hall)), batch(0),
      department(department::Department_of_Computer_Science_and_Engineering) {

    // Initialize char arrays with zeros first
    memset(Name, 0, sizeof(Name));
    memset(tokenNumber, 0, sizeof(tokenNumber));
    memset(mealName, 0, sizeof(mealName));
    memset(comment, 0, sizeof(comment));

    setCharArrayField(Name, email, "Name");
    setCharArrayField(tokenNumber, tokenNum, "tokenNumber");
    setCharArrayField(mealName, meal, "mealName");
    setCharArrayField(comment, commentStr, "comment");
    reviewDate = Date(date);
}

// Getters using StringHelper
string MealReview::getStudentEmail() const { return StringHelper::charArrayToString(Name); }
string MealReview::getTokenNumber() const { return StringHelper::charArrayToString(tokenNumber); }
string MealReview::getMealName() const { return StringHelper::charArrayToString(mealName); }
MealRating MealReview::getRating() const { return rating; }
string MealReview::getComment() const { return StringHelper::charArrayToString(comment); }
string MealReview::getReviewDate() const { return reviewDate.toString(); }
string MealReview::getHallName() const { return hallToString(hallName); }

string MealReview::ratingToString(MealRating rating) {
    static const vector<pair<MealRating, string>> ratingMap = {
        {MealRating::POOR, "Poor"},
        {MealRating::FAIR, "Fair"},
        {MealRating::GOOD, "Good"},
        {MealRating::VERY_GOOD, "Very Good"},
        {MealRating::EXCELLENT, "Excellent"}
    };
    return enumToStringHelper(rating, ratingMap);
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

// ========== TOKEN MANAGER CLASS IMPLEMENTATION ==========

TokenManager::TokenManager() {
    createTokenFolder();
    loadAllTokens();
    loadAllReviews();
}

string TokenManager::buyToken(const string& studentEmail, const string& hallName,
                             MealType mealType, const Meal& meal) {
    string tokenNumber = MealToken::generateTokenNumber();
    Date today;
    Date parsedDate(meal.getDate());
    Date validDate = parsedDate.isValid() ? parsedDate : today;

    MealToken newToken(tokenNumber, studentEmail, meal.getMealName(),
                      mealType, hallName, meal.getPrice(), today, validDate);

    // Use DatabaseManager to add token
    if (DatabaseManager::addActiveToken(newToken)) {
        activeTokens.push_back(newToken);
        newToken.saveToFile(TOKEN_FOLDER);
        return tokenNumber;
    }
    return "";
}

bool TokenManager::useToken(const string& tokenNumber, const string& studentEmail) {
    // Find token in active tokens
    MealToken* token = DatabaseManager::findActiveTokenByID(tokenNumber);
    if (token && token->getStudentEmail() == studentEmail) {
        if (!token->canBeUsed()) return false;

        // Mark as used and move to used tokens
        token->markAsUsed();
        MealToken usedToken = *token;

        // Add to used tokens and remove from active tokens
        if (DatabaseManager::addUsedToken(usedToken) &&
            DatabaseManager::deleteActiveToken(tokenNumber)) {
            // Update local cache
            activeTokens.erase(
                remove_if(activeTokens.begin(), activeTokens.end(),
                    [&](const MealToken& t) { return t.getTokenNumber() == tokenNumber; }),
                activeTokens.end());
            usedTokens.push_back(usedToken);
            return true;
        }
    }
    return false;
}

bool TokenManager::canBuyToken(const string& studentEmail, MealType mealType, const Date& forDate) const {
    return all_of(activeTokens.begin(), activeTokens.end(),
        [&](const MealToken& token) {
            return !(token.getStudentEmail() == studentEmail &&
                    token.getMealType() == mealType &&
                    token.getValidDate().toString() == forDate.toString());
        });
}

bool TokenManager::addReview(const string& studentEmail, const string& tokenNumber,
                            MealRating rating, const string& comment) {
    // Find token in used tokens
    MealToken* token = DatabaseManager::findUsedTokenByID(tokenNumber);
    if (token && token->getStudentEmail() == studentEmail) {
        Date today;
        string mealName = token->getMealName().empty() ? "Meal" : token->getMealName();
        MealReview review(studentEmail, tokenNumber, mealName, rating, comment,
                          today.toString(), token->getHallName());

        // Use DatabaseManager to add review
        if (DatabaseManager::addReview(review)) {
            reviews.push_back(review);
            // Mark token as reviewed
            token->markAsReviewed();
            if (DatabaseManager::updateUsedToken(tokenNumber, *token)) {
                // Update local cache
                for (auto& t : usedTokens) {
                    if (t.getTokenNumber() == tokenNumber) {
                        t.markAsReviewed();
                        break;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

void TokenManager::cleanupExpiredTokens() {
    bool changed = false;
    for (auto& token : activeTokens) {
        if (token.getStatus() == TokenStatus::ACTIVE && token.isExpired()) {
            token.setStatus(TokenStatus::EXPIRED);
            DatabaseManager::updateActiveToken(token.getTokenNumber(), token);
            changed = true;
        }
    }
    if (changed) {
        loadAllTokens(); // Reload to sync
    }
}

vector<MealToken> TokenManager::getStudentTokens(const string& studentEmail) const {
    // Use filterCollection helper to combine both active and used tokens
    auto activeMatches = filterCollection(activeTokens,
        [&](const MealToken& token) { return token.getStudentEmail() == studentEmail; });
    auto usedMatches = filterCollection(usedTokens,
        [&](const MealToken& token) { return token.getStudentEmail() == studentEmail; });

    // Combine results
    activeMatches.insert(activeMatches.end(), usedMatches.begin(), usedMatches.end());
    return activeMatches;
}

void TokenManager::displayStudentTokens(const string& studentEmail) const {
    cout << "\n=== Active Tokens ===" << endl;
    bool hasActive = false;
    for (const auto& token : activeTokens) {
        if (token.getStudentEmail() == studentEmail) {
            token.displayToken();
            hasActive = true;
            cout << "---" << endl;
        }
    }
    if (!hasActive) cout << "No active tokens found." << endl;

    cout << "\n=== Used Tokens ===" << endl;
    bool hasUsed = false;
    for (const auto& token : usedTokens) {
        if (token.getStudentEmail() == studentEmail) {
            token.displayToken();
            hasUsed = true;
            cout << "---" << endl;
        }
    }
    if (!hasUsed) cout << "No used tokens found." << endl;
}

void TokenManager::createTokenFolder() {
    filesystem::create_directories(TOKEN_FOLDER);
}

// Database operations using DatabaseManager
void TokenManager::saveAllTokens() {
    DatabaseManager::saveActiveTokens(activeTokens);
    DatabaseManager::saveUsedTokens(usedTokens);
}

void TokenManager::loadAllTokens() {
    activeTokens = DatabaseManager::loadActiveTokens();
    usedTokens = DatabaseManager::loadUsedTokens();
}

void TokenManager::saveAllReviews() {
    DatabaseManager::saveReviews(reviews);
}

void TokenManager::loadAllReviews() {
    reviews = DatabaseManager::loadReviews();
}

vector<MealReview> TokenManager::getMealReviews(const string& mealName) const {
    return filterCollection(reviews,
        [&](const MealReview& review) { return review.getMealName() == mealName; });
}

vector<MealReview> TokenManager::getHallReviews(const string& hallName) const {
    return filterCollection(reviews,
        [&](const MealReview& review) { return review.getHallName() == hallName; });
}

void TokenManager::displayAllReviews() const {
    if (reviews.empty()) {
        cout << "No reviews available." << endl;
        return;
    }
    cout << "\n=== ALL MEAL REVIEWS ===" << endl;
    for (const auto& review : reviews) {
        review.displayReview();
        cout << "---" << endl;
    }
}

// ========== MEAL UTILS CLASS IMPLEMENTATION ==========

bool MealUtils::isWithinMealTime(MealType type) {
    int hour;
    if (Date::isSimulationActive() && Date::getSimulatedHour() >= 0) {
        hour = Date::getSimulatedHour();
    } else {
        time_t now = time(nullptr);
        tm* ltm = localtime(&now);
        hour = ltm->tm_hour;
    }

    switch (type) {
        case MealType::BREAKFAST: return hour >= 7 && hour <= 10;
        case MealType::LUNCH: return hour >= 12 && hour <= 14;
        case MealType::DINNER: return hour >= 19 && hour <= 22;
        default: return false;
    }
}

string MealUtils::getCurrentTime() {
    int hour, minute;
    if (Date::isSimulationActive() && Date::getSimulatedHour() >= 0) {
        hour = Date::getSimulatedHour();
        minute = 0;
    } else {
        time_t now = time(nullptr);
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
        "Al Beruni Hall", "Meer Mosharraf Hossain Hall", "Shaheed Salam Barkat Hall",
        "AFM Kamaluddin Hall", "Moulana Bhasani Hall", "Bangabondhu Sheikh Majibur Rahman Hall",
        "Jatiya Kabi Kazi Nazrul Islam Hall", "Rabindra Nath Tagore Hall", "Shahid Tajuddin Ahmed Hall",
        "Shahid Sheikh Russel Hall", "Shaheed Rafiq Jabbar Hall", "Nawab Faizunnesa Hall",
        "Fazilatunnesa Hall", "Jahanara Imam Hall", "Preetilata Hall", "Begum Khaleda Zia Hall",
        "Sheikh Hasina Hall", "Bir Pratik Taramon Bibi Hall"
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
