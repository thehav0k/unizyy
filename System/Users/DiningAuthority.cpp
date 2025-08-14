//
// Created by Md. Asif Khan on 11/8/25.
//

#include "DiningAuthority.h"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <cstring>
#include "../../Core/Database/DatabaseManager.h"
#include "../../Core/Utils/StringHelper.h"

using namespace std;
using namespace std::filesystem;

// Constructors
DiningAuthority::DiningAuthority(const string& email, const string& password, const string& name,
                                const string& hallName)
    : User(email, password), hallname(stringToHalls(hallName)) {

    StringHelper::stringToCharArray(name, this->name);
}

DiningAuthority::DiningAuthority() : User(), hallname(Halls::Al_Beruni_Hall) {
    name[0] = '\0';
}

// Getters
string DiningAuthority::getName() const {
    return StringHelper::charArrayToString(name);
}

string DiningAuthority::getHallName() const {
    return hallToString(hallname);
}

// Setters
void DiningAuthority::setName(const string& newName) {
    StringHelper::stringToCharArray(newName, this->name);
}

void DiningAuthority::setHallName(const string& newHallName) {
    this->hallname = stringToHalls(newHallName);
}

// Meal Management Operations
void DiningAuthority::createMeal(const string& mealName, const string& description, MealType type,
                                double price, int quantity, const string& date, const string& time) {

    string currentHall = getHallName();
    Meal newMeal(mealName, description, type, price, quantity, date, time, currentHall);

    // Save to database
    if (newMeal.saveMealToDatabase()) {
        cout << "✅ Meal '" << mealName << "' created successfully for " << currentHall << "!" << endl;
        cout << "Details: " << description << " | Price: $" << fixed << setprecision(2) << price
             << " | Quantity: " << quantity << endl;
    } else {
        cout << "❌ Failed to save meal to database!" << endl;
    }
}

void DiningAuthority::updateMealAvailability(const string& mealName, bool available) {
    vector<Meal> meals = Meal::loadMealsByHall(getHallName());
    bool found = false;

    for (auto& meal : meals) {
        if (meal.getMealName() == mealName) {
            meal.setIsAvailable(available);
            meal.saveMealToDatabase();
            found = true;
            cout << "✅ Meal availability updated: " << mealName << " is now "
                 << (available ? "available" : "unavailable") << endl;
            break;
        }
    }

    if (!found) {
        cout << "❌ Meal not found: " << mealName << endl;
    }
}

void DiningAuthority::updateMealQuantity(const string& mealName, int newQuantity) {
    vector<Meal> meals = Meal::loadMealsByHall(getHallName());
    bool found = false;

    for (auto& meal : meals) {
        if (meal.getMealName() == mealName) {
            int oldQuantity = meal.getAvailableQuantity();
            meal.setAvailableQuantity(newQuantity);
            meal.saveMealToDatabase();
            found = true;
            cout << "✅ Meal quantity updated: " << mealName << " | Old: " << oldQuantity
                 << " → New: " << newQuantity << " servings" << endl;
            break;
        }
    }

    if (!found) {
        cout << "❌ Meal not found: " << mealName << endl;
    }
}

void DiningAuthority::updateMealPrice(const string& mealName, double newPrice) {
    vector<Meal> meals = Meal::loadMealsByHall(getHallName());
    bool found = false;

    for (auto& meal : meals) {
        if (meal.getMealName() == mealName) {
            double oldPrice = meal.getPrice();
            meal.setPrice(newPrice);
            meal.saveMealToDatabase();
            found = true;
            cout << "✅ Meal price updated: " << mealName << " | Old: $" << fixed << setprecision(2) << oldPrice
                 << " → New: $" << newPrice << endl;
            break;
        }
    }

    if (!found) {
        cout << "❌ Meal not found: " << mealName << endl;
    }
}

void DiningAuthority::removeMeal(const string& mealName) {
    vector<Meal> meals = Meal::loadMealsByHall(getHallName());
    bool found = false;

    for (const auto& meal : meals) {
        if (meal.getMealName() == mealName) {
            if (Meal::deleteMealFromDatabase(meal.getDate(), meal.getHallName(), meal.getMealType())) {
                cout << "✅ Meal '" << mealName << "' removed successfully!" << endl;
                found = true;
            } else {
                cout << "❌ Failed to remove meal from database!" << endl;
            }
            break;
        }
    }

    if (!found) {
        cout << "❌ Meal not found: " << mealName << endl;
    }
}

// View Operations
void DiningAuthority::viewAllMeals() const {
    vector<Meal> meals = Meal::loadMealsByHall(getHallName());

    if (meals.empty()) {
        cout << "📋 No meals available in " << getHallName() << endl;
        return;
    }

    cout << "\n🍽️ === MEALS IN " << getHallName() << " ===" << endl;
    cout << "Total Meals: " << meals.size() << endl;
    cout << "Manager: " << getName() << endl;
    cout << string(80, '=') << endl;

    for (size_t i = 0; i < meals.size(); ++i) {
        cout << "\n--- Meal " << (i + 1) << " ---" << endl;
        meals[i].displayMeal();
    }
}

void DiningAuthority::viewMealsByType(MealType type) const {
    vector<Meal> allMeals = Meal::loadMealsByHall(getHallName());
    vector<Meal> mealsOfType;

    for (const auto& meal : allMeals) {
        if (meal.getMealType() == type) {
            mealsOfType.push_back(meal);
        }
    }

    if (mealsOfType.empty()) {
        cout << "📋 No " << Meal::mealTypeToString(type) << " meals found in " << getHallName() << endl;
        return;
    }

    cout << "\n🍽️ === " << Meal::mealTypeToString(type) << " MEALS IN " << getHallName() << " ===" << endl;
    cout << "Found: " << mealsOfType.size() << " meals" << endl;
    cout << string(80, '=') << endl;

    for (size_t i = 0; i < mealsOfType.size(); ++i) {
        cout << "\n--- Meal " << (i + 1) << " ---" << endl;
        mealsOfType[i].displayMeal();
    }
}

void DiningAuthority::viewMealsByDate(const Date& date) const {
    vector<Meal> meals = Meal::loadMealsByDate(date.toString());
    vector<Meal> hallMeals;

    for (const auto& meal : meals) {
        if (meal.getHallName() == getHallName()) {
            hallMeals.push_back(meal);
        }
    }

    cout << "\n📅 === MEALS FOR " << date.toString() << " IN " << getHallName() << " ===" << endl;

    if (hallMeals.empty()) {
        cout << "📋 No meals scheduled for " << date.toString() << " in " << getHallName() << endl;
    } else {
        for (const auto& meal : hallMeals) {
            meal.displayMeal();
            cout << endl;
        }
    }
}

void DiningAuthority::viewTodaysMeals() const {
    Date today;
    viewMealsByDate(today);
}

void DiningAuthority::viewUpcomingMeals() const {
    Date today;
    vector<Meal> meals = Meal::loadMealsByHall(getHallName());

    cout << "\n📅 === UPCOMING MEALS IN " << getHallName() << " ===" << endl;
    bool found = false;

    for (const auto& meal : meals) {
        Date mealDate(meal.getDate());
        if (mealDate > today) {
            meal.displayMeal();
            cout << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "📋 No upcoming meals scheduled in " << getHallName() << endl;
    }
}

// Simplified token and review methods
void DiningAuthority::viewActiveTokens() const {
    cout << "\n🎫 === ACTIVE TOKENS FOR " << getHallName() << " ===" << endl;
    cout << "Note: Token management system not yet implemented." << endl;
    cout << "This feature will be available in future updates." << endl;
}

void DiningAuthority::viewUsedTokens() const {
    cout << "\n🎫 === USED TOKENS FOR " << getHallName() << " ===" << endl;
    cout << "Note: Token management system not yet implemented." << endl;
    cout << "This feature will be available in future updates." << endl;
}

void DiningAuthority::viewFoodReviews() const {
    cout << "\n⭐ === FOOD REVIEWS FOR " << getHallName() << " ===" << endl;
    cout << "Note: Review management system not yet implemented." << endl;
    cout << "This feature will be available in future updates." << endl;
}

void DiningAuthority::viewReviewsByMeal(const string& mealName) const {
    cout << "\n⭐ === REVIEWS FOR " << mealName << " ===" << endl;
    cout << "Note: Review management system not yet implemented." << endl;
    cout << "This feature will be available in future updates." << endl;
}

void DiningAuthority::viewHallReviews() const {
    cout << "\n⭐ === REVIEWS FOR " << getHallName() << " ===" << endl;
    cout << "Note: Review management system not yet implemented." << endl;
    cout << "This feature will be available in future updates." << endl;
}

void DiningAuthority::respondToReview(const string& tokenNumber, const string& response) {
    cout << "\n📝 === RESPONDING TO REVIEW ===" << endl;
    cout << "Token: " << tokenNumber << endl;
    cout << "Response: " << response << endl;
    cout << "Hall: " << getHallName() << endl;
    cout << "Note: Review response system not yet implemented." << endl;
    cout << "This feature will be available in future updates." << endl;
}

// Notice Management (simplified)
void DiningAuthority::postNotice(const string& title, const string& content) {
    cout << "\n📢 === POSTING NOTICE ===" << endl;
    cout << "Title: " << title << endl;
    cout << "Content: " << content << endl;
    cout << "Hall: " << getHallName() << endl;
    cout << "✅ Notice posted successfully!" << endl;
    cout << "Note: Notice persistence not yet implemented." << endl;
}

void DiningAuthority::viewNotices() const {
    cout << "\n📢 === NOTICES FOR " << getHallName() << " ===" << endl;
    cout << "Note: Notice management system not yet implemented." << endl;
    cout << "This feature will be available in future updates." << endl;
}

void DiningAuthority::removeNotice(const string& title) {
    cout << "\n📢 === REMOVING NOTICE ===" << endl;
    cout << "Title: " << title << endl;
    cout << "✅ Notice removed successfully!" << endl;
    cout << "Note: Notice persistence not yet implemented." << endl;
}

// Analytics and Reports (simplified)
void DiningAuthority::generateDailyReport(const Date& date) const {
    cout << "\n📊 === DAILY REPORT FOR " << date.toString() << " ===" << endl;
    cout << "Hall: " << getHallName() << endl;
    cout << "Manager: " << getName() << endl;

    // Count meals for the date
    vector<Meal> meals = Meal::loadMealsByDate(date.toString());
    int mealsCount = 0;
    for (const auto& meal : meals) {
        if (meal.getHallName() == getHallName()) {
            mealsCount++;
        }
    }

    cout << "Meals scheduled: " << mealsCount << endl;
    cout << "Total hall meals: " << Meal::loadMealsByHall(getHallName()).size() << endl;
}

void DiningAuthority::viewPopularMeals() const {
    cout << "\n🏆 === POPULAR MEALS IN " << getHallName() << " ===" << endl;
    cout << "Note: Popularity analytics not yet implemented." << endl;
    cout << "This feature will be available with token usage data." << endl;
}

void DiningAuthority::viewRevenueReport() const {
    cout << "\n💰 === REVENUE REPORT FOR " << getHallName() << " ===" << endl;

    vector<Meal> meals = Meal::loadMealsByHall(getHallName());
    double totalRevenue = 0.0;
    for (const auto& meal : meals) {
        totalRevenue += meal.getPrice();
    }

    cout << "Total meals: " << meals.size() << endl;
    cout << "Estimated revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
    cout << "Note: Actual revenue tracking requires token sales data." << endl;
}

// Inventory Management
void DiningAuthority::updateInventory(const string& mealName, int consumedQuantity) {
    vector<Meal> meals = Meal::loadMealsByHall(getHallName());
    bool found = false;

    for (auto& meal : meals) {
        if (meal.getMealName() == mealName) {
            int currentQuantity = meal.getAvailableQuantity();
            int newQuantity = max(0, currentQuantity - consumedQuantity);
            meal.setAvailableQuantity(newQuantity);
            meal.saveMealToDatabase();
            found = true;

            cout << "✅ Inventory updated for " << mealName << endl;
            cout << "Consumed: " << consumedQuantity << " | Remaining: " << newQuantity << endl;
            break;
        }
    }

    if (!found) {
        cout << "❌ Meal not found: " << mealName << endl;
    }
}

void DiningAuthority::viewLowStockMeals() const {
    cout << "\n⚠️ === LOW STOCK MEALS IN " << getHallName() << " ===" << endl;

    vector<Meal> meals = Meal::loadMealsByHall(getHallName());
    bool foundLowStock = false;

    for (const auto& meal : meals) {
        if (meal.getAvailableQuantity() < 10) { // Consider less than 10 as low stock
            cout << "🔴 " << meal.getMealName() << " - Only " << meal.getAvailableQuantity() << " left" << endl;
            foundLowStock = true;
        }
    }

    if (!foundLowStock) {
        cout << "✅ All meals have sufficient stock!" << endl;
    }
}

void DiningAuthority::restockMeal(const string& mealName, int additionalQuantity) {
    vector<Meal> meals = Meal::loadMealsByHall(getHallName());
    bool found = false;

    for (auto& meal : meals) {
        if (meal.getMealName() == mealName) {
            int currentQuantity = meal.getAvailableQuantity();
            int newQuantity = currentQuantity + additionalQuantity;
            meal.setAvailableQuantity(newQuantity);
            meal.saveMealToDatabase();
            found = true;

            cout << "✅ " << mealName << " restocked!" << endl;
            cout << "Added: " << additionalQuantity << " | New total: " << newQuantity << endl;
            break;
        }
    }

    if (!found) {
        cout << "❌ Meal not found: " << mealName << endl;
    }
}

// Static database operations using DatabaseManager
vector<DiningAuthority> DiningAuthority::loadAllAuthorities() {
    return DatabaseManager::loadDiningAuthorities();
}

bool DiningAuthority::addAuthorityToDB(const DiningAuthority& authority) {
    return DatabaseManager::addDiningAuthority(authority);
}

bool DiningAuthority::updateAuthorityInDB(const string& email, const DiningAuthority& updatedAuthority) {
    return DatabaseManager::updateDiningAuthority(email, updatedAuthority);
}

bool DiningAuthority::deleteAuthorityFromDB(const string& email) {
    return DatabaseManager::deleteDiningAuthority(email);
}

DiningAuthority* DiningAuthority::findAuthorityByEmail(const string& email) {
    return DatabaseManager::findDiningAuthorityByEmail(email);
}

// Utility Functions
vector<Meal> DiningAuthority::getMealsByHall() const {
    return Meal::loadMealsByHall(getHallName());
}

vector<Meal> DiningAuthority::getMealsByType(MealType type) const {
    vector<Meal> allMeals = Meal::loadMealsByHall(getHallName());
    vector<Meal> result;

    for (const auto& meal : allMeals) {
        if (meal.getMealType() == type) {
            result.push_back(meal);
        }
    }
    return result;
}

vector<Meal> DiningAuthority::getAvailableMeals() const {
    vector<Meal> allMeals = Meal::loadMealsByHall(getHallName());
    vector<Meal> result;

    for (const auto& meal : allMeals) {
        if (meal.getIsAvailable()) {
            result.push_back(meal);
        }
    }
    return result;
}

bool DiningAuthority::canManageHall(const string& hallName) const {
    return getHallName() == hallName;
}

// Override virtual functions from User
void DiningAuthority::display() const {
    cout << "\n🏛️ === DINING AUTHORITY PROFILE ===" << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Hall: " << getHallName() << endl;
    cout << "🍽️ Managed Meals: " << getMealsByHall().size() << endl;
    cout << "=================================" << endl;
}
