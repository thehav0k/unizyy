#include "DiningAuthority.h"
#include "../../Core/Utils/StringHelper.h"
#include "../../Core/Database/DatabaseManager.h"
#include "../Modules/Meal/meal.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

//constructors
DiningAuthority::DiningAuthority() : User(), hallname(Halls::Al_Beruni_Hall) {
    name[0] = '\0';
}

DiningAuthority::DiningAuthority(const string &email, const string &password, const string &name,const string &hallName)
        : User(email, password), hallname(stringToHalls(hallName)) {
    StringHelper::stringToCharArray(name, this->name);
}

//getters
string DiningAuthority::getName() const { return StringHelper::charArrayToString(name); }
string DiningAuthority::getHallName() const { return hallToString(hallname); }
Halls DiningAuthority::getHall() const { return hallname; }

//setters
void DiningAuthority::setName(const string &newName) { StringHelper::stringToCharArray(newName, name); }
void DiningAuthority::setHallName(const string &newHallName) { hallname = stringToHalls(newHallName); }
void DiningAuthority::setHall(Halls hall) { hallname = hall; }

//profile display
//sob info to char arr te so print korar age string banaya nite hbe
void DiningAuthority::display() const {
    cout << "Dining Authority Details:" << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Hall: " << getHallName() << endl;
}

// meal er sob operation
// just use meal class
void DiningAuthority::createMeal(const string &mealName, const string &description, MealType type,
                                 double price, int quantity, const string &date, const string &time) {
    Meal meal(mealName, description, type, price, quantity, date, time, getHallName());
    bool ok = Meal::addMeal(meal);
    cout << (ok ? "Meal created successfully!" : "Failed to create meal.") << endl;
}
//meal id use kore update kor
//index er mto display korbe
//okhan theke select kore update
void DiningAuthority::updateMeal(int mealId, const string &newName, const string &newDescription,
                                 MealType newType, double newPrice, int newQuantity) {
    vector<Meal> meals = DatabaseManager::getMealsByHall(getHallName());
    if (mealId < 0 || mealId >= static_cast<int>(meals.size())) {
        cout << "Invalid meal ID." << endl;
        return;
    }

    Meal target = meals[mealId];
    if (StringHelper::isValidString(newName)) target.setMealName(newName);
    if (StringHelper::isValidString(newDescription)) target.setDescription(newDescription);
    target.setMealType(newType);
    if (newPrice >= 0) target.setPrice(newPrice);
    if (newQuantity >= 0) target.setAvailableQuantity(newQuantity);

    bool ok = Meal::updateMeal(target.getDate(), target.getHallName(), meals[mealId].getMealType(), target);
    cout << (ok ? "Meal updated successfully!" : "Failed to update meal.") << endl;
}
//delete o same vabe hbe
void DiningAuthority::deleteMeal(int mealId) {
    vector<Meal> meals = DatabaseManager::getMealsByHall(getHallName());
    if (mealId < 0 || mealId >= static_cast<int>(meals.size())) {
        cout << "Invalid meal ID." << endl;
        return;
    }

    Meal target = meals[mealId];
    bool ok = Meal::deleteMealFromDatabase(target.getDate(), target.getHallName(), target.getMealType());
    cout << (ok ? "Meal deleted successfully!" : "Failed to delete meal.") << endl;
}

void DiningAuthority::viewAllMeals() const {
    vector<Meal> meals = DatabaseManager::getMealsByHall(getHallName());
    if (meals.empty()) {
        cout << "No meals found for this hall." << endl;
        return;
    }

    cout << "\n=== ALL MEALS FOR " << getHallName() << " ===" << endl;
    int idx = 0;
    for (const auto &m: meals) {
        cout << "[ID: " << idx++ << "]" << endl;
        m.displayMeal();
        cout << "---" << endl;
    }
}

//Review gula database theke niye display
void DiningAuthority::viewFoodReviews() const {
    vector<MealReview> reviews = DatabaseManager::loadReviews();
    cout << "\n=== MEAL REVIEWS FOR " << getHallName() << " ===" << endl;
    int count = 0;
    for (const auto &r: reviews) {
        if (r.getHallName() == getHallName()) {
            r.displayReview();
            cout << "---" << endl;
            count++;
        }
    }
    if (!count) cout << "No reviews yet." << endl;
}

//notice normal notice er motoi
void DiningAuthority::addNotice(const string& title, const string& message) {
    if (title.empty() || message.empty()) {
        cout << "Error: Title and message cannot be empty." << endl;
        return;
    }
    Date currentDate = Date::getCurrentDate();
    Notice newNotice(title, message, currentDate);

    bool ok = DatabaseManager::addNotice(newNotice);
    cout << (ok ? "Notice added successfully!" : "Failed to add notice.") << endl;
}
