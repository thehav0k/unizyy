//
// Created by Md. Asif Khan on 11/8/25.
//

#ifndef DININGAUTHORITY_H
#define DININGAUTHORITY_H

#include "user.h"
#include "../Modules/Meal/meal.h"
#include "../../Core/Models/hall.h"
#include "../../Core/Models/date.h"
#include <vector>
#include <fstream>

// Forward declarations
class TokenManager;

class DiningAuthority final : public User {
private:
    char name[100];
    Halls hallname;

public:
    // Constructors
    DiningAuthority(const string& email, const string& password, const string& name,
                   const string& hallName);

    DiningAuthority(); // Default constructor

    // Getters
    [[nodiscard]] string getName() const;
    [[nodiscard]] string getHallName() const;

    // Setters
    void setName(const string& name);
    void setHallName(const string& hallName);

    // Meal Management Operations
    void createMeal(const string& mealName, const string& description, MealType type,
                   double price, int quantity, const string& date, const string& time);
    void updateMealAvailability(const string& mealName, bool available);
    void updateMealQuantity(const string& mealName, int newQuantity);
    void updateMealPrice(const string& mealName, double newPrice);
    void removeMeal(const string& mealName);

    // View Operations
    void viewAllMeals() const;
    void viewMealsByType(MealType type) const;
    void viewMealsByDate(const Date& date) const;
    void viewTodaysMeals() const;
    void viewUpcomingMeals() const;

    // Simplified methods without complex dependencies
    void viewActiveTokens() const;
    void viewUsedTokens() const;
    void viewFoodReviews() const;
    void viewReviewsByMeal(const string& mealName) const;
    void viewHallReviews() const;
    void respondToReview(const string& tokenNumber, const string& response);

    // Notice Management
    void postNotice(const string& title, const string& content);
    void viewNotices() const;
    void removeNotice(const string& title);

    // Analytics and Reports - Simplified
    void generateDailyReport(const Date& date) const;
    void viewPopularMeals() const;
    void viewRevenueReport() const;

    // Inventory Management
    void updateInventory(const string& mealName, int consumedQuantity);
    void viewLowStockMeals() const;
    void restockMeal(const string& mealName, int additionalQuantity);

    // Static database operations using DatabaseManager
    static vector<DiningAuthority> loadAllAuthorities();
    static bool addAuthorityToDB(const DiningAuthority& authority);
    static bool updateAuthorityInDB(const string& email, const DiningAuthority& updatedAuthority);
    static bool deleteAuthorityFromDB(const string& email);
    static DiningAuthority* findAuthorityByEmail(const string& email);

    // Utility Functions
    [[nodiscard]] vector<Meal> getMealsByHall() const;
    [[nodiscard]] vector<Meal> getMealsByType(MealType type) const;
    [[nodiscard]] vector<Meal> getAvailableMeals() const;
    [[nodiscard]] bool canManageHall(const string& hallName) const;

    // Override virtual functions from User
    void display() const override;
};

#endif //DININGAUTHORITY_H
