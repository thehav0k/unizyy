//
// Created by Md. Asif Khan on 11/8/25.
//

#ifndef UNIZYY_DININGAUTHORITY_H
#define UNIZYY_DININGAUTHORITY_H

#include "user.h"
#include "../../Core/Models/hall.h"
#include "../Modules/Meal/meal.h" // added for MealType
#include <string>
#include <vector>

using namespace std;

class DiningAuthority : public User {
private:
    char name[100];
    Halls hallname;

public:
    // Constructors
    DiningAuthority();
    DiningAuthority(const string& email, const string& password, const string& name,
                   const string& hallName);

    // Getters
    string getName() const;
    string getHallName() const;
    Halls getHall() const;

    // Setters
    void setName(const string& newName);
    void setHallName(const string& newHallName);
    void setHall(Halls hall);

    // Display methods
    void display() const override;
    void displayDiningMenu() const;


    void createMeal(const string& mealName, const string& description, MealType type,
                   double price, int quantity, const string& date, const string& time);
    void updateMeal(int mealId, const string& newName, const string& newDescription,
                   MealType newType, double newPrice, int newQuantity);
    void deleteMeal(int mealId);
    void displayAllMeals() const;
    void searchMealsByName(const string& name) const;
    void searchMealsByType(MealType type) const;
    void generateMealReport() const;
    void DisplayTokenSellsReport() const; // student buys meal token which will be saved in the database and will be displayed
    void ViewFoodReviews() const; // students review for meal
};

#endif //UNIZYY_DININGAUTHORITY_H
