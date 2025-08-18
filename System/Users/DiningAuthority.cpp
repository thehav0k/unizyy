// DiningAuthority.cpp

#include "DiningAuthority.h"
#include "../../Core/Utils/StringHelper.h"
#include "../../Core/Database/DatabaseManager.h"
#include "../Modules/Meal/meal.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

// ===== Helper utilities (anonymous namespace for internal linkage) =====
namespace {
    string mealTypeToString(MealType type) { return Meal::mealTypeToString(type); }

    // Case-insensitive find
    bool icontains(const string &haystack, const string &needle) {
        string h = haystack, n = needle;
        transform(h.begin(), h.end(), h.begin(), ::tolower);
        transform(n.begin(), n.end(), n.begin(), ::tolower);
        return h.find(n) != string::npos;
    }

    string hallToKey(const Halls &h) { return hallToString(h); }
}

// ===== Constructors =====
DiningAuthority::DiningAuthority() : User(), hallname(Halls::Al_Beruni_Hall) {
    name[0] = '\0';
}

DiningAuthority::DiningAuthority(const string &email, const string &password, const string &name,
                                 const string &hallName)
        : User(email, password), hallname(stringToHalls(hallName)) {
    StringHelper::stringToCharArray(name, this->name);
}

// ===== Getters =====
string DiningAuthority::getName() const { return StringHelper::charArrayToString(name); }
string DiningAuthority::getHallName() const { return hallToString(hallname); }
Halls DiningAuthority::getHall() const { return hallname; }

// ===== Setters =====
void DiningAuthority::setName(const string &newName) { StringHelper::stringToCharArray(newName, name); }
void DiningAuthority::setHallName(const string &newHallName) { hallname = stringToHalls(newHallName); }
void DiningAuthority::setHall(Halls hall) { hallname = hall; }

// ===== Display =====
void DiningAuthority::display() const {
    cout << "Dining Authority Details:" << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Hall: " << getHallName() << endl;
}

void DiningAuthority::displayDiningMenu() const {
    vector<Meal> meals = DatabaseManager::getMealsByHall(getHallName());
    if (meals.empty()) {
        cout << "No meals configured for hall: " << getHallName() << endl;
        return;
    }
    cout << "\n=== DINING MENU (" << getHallName() << ") ===" << endl;
    for (const auto &m: meals) {
        m.displayMeal();
        cout << "---" << endl;
    }
}

// ===== Meal CRUD (Hall scoped) =====
void DiningAuthority::createMeal(const string &mealName, const string &description, MealType type,
                                 double price, int quantity, const string &date, const string &time) {
    Meal meal(mealName, description, type, price, quantity, date, time, getHallName());
    bool ok = Meal::addMeal(meal);
    cout << (ok ? "Meal created." : "Failed to create meal.") << endl;
}

void DiningAuthority::updateMeal(int mealId, const string &newName, const string &newDescription,
                                 MealType newType, double newPrice, int newQuantity) {
    // Interpret mealId as index among this hall's meals (0-based)
    vector<Meal> meals = DatabaseManager::getMealsByHall(getHallName());
    if (mealId < 0 || mealId >= static_cast<int>(meals.size())) {
        cout << "Meal ID out of range." << endl;
        return;
    }
    Meal target = meals[mealId];
    if (StringHelper::isValidString(newName)) target.setMealName(newName);
    if (StringHelper::isValidString(newDescription)) target.setDescription(newDescription);
    target.setMealType(newType);
    if (newPrice >= 0) target.setPrice(newPrice);
    if (newQuantity >= 0) target.setAvailableQuantity(newQuantity);

    bool ok = Meal::updateMeal(target.getDate(), target.getHallName(), meals[mealId].getMealType(), target);
    cout << (ok ? "Meal updated." : "Failed to update meal.") << endl;
}

void DiningAuthority::deleteMeal(int mealId) {
    vector<Meal> meals = DatabaseManager::getMealsByHall(getHallName());
    if (mealId < 0 || mealId >= static_cast<int>(meals.size())) {
        cout << "Meal ID out of range." << endl;
        return;
    }
    Meal target = meals[mealId];
    bool ok = Meal::deleteMealFromDatabase(target.getDate(), target.getHallName(), target.getMealType());
    cout << (ok ? "Meal deleted." : "Failed to delete meal.") << endl;
}

void DiningAuthority::displayAllMeals() const {
    vector<Meal> meals = DatabaseManager::getMealsByHall(getHallName());
    if (meals.empty()) {
        cout << "No meals found for this hall." << endl;
        return;
    }
    cout << "\n=== ALL MEALS FOR HALL: " << getHallName() << " ===" << endl;
    int idx = 0;
    for (const auto &m: meals) {
        cout << "[#" << idx++ << "]" << endl;
        m.displayMeal();
        cout << "---" << endl;
    }
}

void DiningAuthority::searchMealsByName(const string &nameQuery) const {
    vector<Meal> meals = DatabaseManager::getMealsByHall(getHallName());
    cout << "\nSearch Results for '" << nameQuery << "':" << endl;
    int matches = 0;
    for (const auto &m: meals) {
        if (icontains(m.getMealName(), nameQuery)) {
            m.displayMeal();
            cout << "---" << endl;
            matches++;
        }
    }
    if (!matches) cout << "No meals matched the query." << endl;
}

void DiningAuthority::searchMealsByType(MealType type) const {
    vector<Meal> meals = DatabaseManager::getMealsByHall(getHallName());
    cout << "\nMeals of type: " << mealTypeToString(type) << endl;
    int matches = 0;
    for (const auto &m: meals) {
        if (m.getMealType() == type) {
            m.displayMeal();
            cout << "---" << endl;
            matches++;
        }
    }
    if (!matches) cout << "No meals of this type." << endl;
}

void DiningAuthority::generateMealReport() const {
    vector<Meal> meals = DatabaseManager::getMealsByHall(getHallName());
    cout << "\n=== MEAL REPORT (" << getHallName() << ") ===" << endl;
    cout << "Total meals configured: " << meals.size() << endl;

    size_t breakfast=0, lunch=0, dinner=0;
    for (const auto &m: meals) {
        switch (m.getMealType()) {
            case MealType::BREAKFAST: breakfast++; break;
            case MealType::LUNCH: lunch++; break;
            case MealType::DINNER: dinner++; break;
        }
    }
    cout << "Breakfast entries: " << breakfast << endl;
    cout << "Lunch entries: " << lunch << endl;
    cout << "Dinner entries: " << dinner << endl;
}

void DiningAuthority::DisplayTokenSellsReport() const {
    // Aggregate token sales for this hall
    vector<MealToken> active = DatabaseManager::loadActiveTokens();
    vector<MealToken> used = DatabaseManager::loadUsedTokens();

    size_t activeCount=0, usedCount=0, expiredCount=0, reviewedCount=0;
    double revenue=0.0;

    auto process = [&](const vector<MealToken>& tokens){
        for (const auto &t: tokens) {
            if (t.getHallName() != getHallName()) continue;
            revenue += t.getPaidAmount();
            TokenStatus st = t.getStatus();
            if (st == TokenStatus::ACTIVE) {
                if (t.isExpired()) expiredCount++; else activeCount++;
            } else if (st == TokenStatus::USED) usedCount++;
            else if (st == TokenStatus::EXPIRED) expiredCount++;
            else if (st == TokenStatus::REVIEWED) reviewedCount++;
        }
    };
    process(active);
    process(used); // reviewed tokens are among used list saved differently

    cout << "\n=== TOKEN SALES REPORT (" << getHallName() << ") ===" << endl;
    cout << "Revenue: ৳" << fixed << setprecision(2) << revenue << endl;
    cout << "Active Tokens: " << activeCount << endl;
    cout << "Used Tokens: " << usedCount << endl;
    cout << "Reviewed Tokens: " << reviewedCount << endl;
    cout << "Expired Tokens: " << expiredCount << endl;
}

void DiningAuthority::ViewFoodReviews() const {
    vector<MealReview> reviews = DatabaseManager::loadReviews();
    cout << "\n=== MEAL REVIEWS (" << getHallName() << ") ===" << endl;
    int count = 0;
    for (const auto &r: reviews) {
        if (r.getHallName() == getHallName()) {
            r.displayReview();
            cout << "---" << endl;
            count++;
        }
    }
    if (!count) cout << "No reviews for this hall yet." << endl;
}
