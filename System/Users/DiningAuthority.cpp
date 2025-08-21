// DiningAuthority.cpp

#include "DiningAuthority.h"
#include "../../Core/Utils/StringHelper.h"
#include "../../Core/Database/DatabaseManager.h"
#include "../Modules/Meal/meal.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;


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
                                 MealType newType, double newPrice, int newQuantity)
}

void DiningAuthority::deleteMeal(int mealId) {
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
}

void DiningAuthority::generateMealReport() const {
    cout << "Dinner entries: " << dinner << endl;
}

void DiningAuthority::DisplayTokenSellsReport() const {
}
