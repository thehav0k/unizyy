//
// Created by Md. Asif Khan on 11/8/25.
// Meal Management System Header
//

#ifndef MEAL_H
#define MEAL_H

#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <random>

#include "../../../Core/Models/department.h"
#include "../../../Core/Models/date.h"
#include "../../../Core/Models/hall.h"

// how things will work
// Student can buy tokens only the before they can use
// While logged in they can see Meal details and price of every type
// they can choose to buy tokens for breakfast, lunch, dinner or anytype together or individual
// They can buy tokens from other halls as well but they are rocommended to buy from their hall
// they can take meals by using their tokens
// After eating they can review meals
// We'll use Core/models and other classes to make code size as small as possible but readable
// UI will be managed by Interface folder's classes
// Dining Authority can publish notice,Add meal details(There will be default meal for every day but they can change it)
// And Sell tokens, see review
// I'm using char array for those variable which needs to be written and read in files
// Also using database manager to reduce code size and manage fineness
using namespace std;

enum class MealType {
    BREAKFAST = 0,
    LUNCH = 1,
    DINNER = 2
};

enum class MealRating {
    POOR = 1,
    FAIR = 2,
    GOOD = 3,
    VERY_GOOD = 4,
    EXCELLENT = 5
};

enum class TokenStatus {
    ACTIVE = 0,
    USED = 1,
    EXPIRED = 2,
    REVIEWED = 3
};

class Meal {
private:
    char MealPackage[300]; // it will have three packages for each mealtype like (Rice, Chicken Curry, Lentils) / (Rice, Fish, Vegetables) etc
    MealType mealType;
    double price; // price in bdt
    int availableQuantity;
    bool isAvailable;
    char date[12]; // Date class er object ke char array te rakhar jnno
    char time[10];
    Halls hallName;

    // Caching (similar to DatabaseManager pattern)
    static vector<Meal> cachedMeals;          // in-memory cache
    static bool mealsLoaded;                  // guard to ensure single load
    static void loadMealsIntoCache();         // internal load w/ legacy support
    static void saveMealsToDisk();            // full rewrite

public:
    // Constructors
    Meal(const string& name, const string& desc, MealType type, double price,
         int quantity, const string& date, const string& time, const string& hall);
    Meal();

    // Getters
    string getMealName() const;
    string getDescription() const;
    MealType getMealType() const;
    double getPrice() const;
    int getAvailableQuantity() const;
    bool getIsAvailable() const;
    string getDate() const;
    string getTime() const;
    string getHallName() const;

    // Setters
    void setMealName(const string& name);
    void setDescription(const string& desc);
    void setMealType(MealType type);
    void setPrice(double price);
    void setAvailableQuantity(int quantity);
    void setIsAvailable(bool available);
    void setDate(const string& date);
    void setTime(const string& time);
    void setHallName(const string& hall);

    // Meal operations
    bool orderMeal(int quantity = 1);
    void displayMeal() const;
    bool isExpired() const;

    // Static utility functions
    static string mealTypeToString(MealType type);
    static MealType stringToMealType(const string& typeStr);

    // Static database management functions
    static void initializeMealDatabase();
    static void ensureMealsLoaded();
    static const vector<Meal>& getCachedMeals();
    static size_t getMealCount();
    static vector<Meal> loadAllMealsFromDatabase();
    static vector<Meal> loadMealsByHall(const string& hallName);
    static vector<Meal> loadMealsByDate(const string& date);
    static vector<Meal> loadMealsByType(MealType type);

    // CRUD operations
    static bool addMeal(const Meal& meal);
    static bool updateMeal(const string& date, const string& hallName, MealType type, const Meal& updatedMeal);
    static bool deleteMealFromDatabase(const string& date, const string& hallName, MealType type);
    static void displayAllMeals();

    // Instance method for saving current meal
    bool saveMealToDatabase() const;
};

class MealToken {
private:
    char tokenNumber[20];     // Unique token identifier
    char studentEmail[120];   // Student email owning the token
    char mealName[120];       // Cached meal name
    MealType mealType;
    Halls hallName;
    double paidAmount;
    Date purchaseDate;
    Date validDate;           // Date when meal can be taken
    TokenStatus status;
    char purchaseTime[10];    // Time when token was purchased

public:
    // Constructors
    MealToken();
    MealToken(const string& tokenNum, const string& studentEmail,
              const string& mealName, MealType type, const string& hall,
              double amount, const Date& purchaseDate, const Date& validDate);

    // Getters
    string getTokenNumber() const;
    string getStudentEmail() const;
    string getMealName() const;
    MealType getMealType() const;
    string getHallName() const;
    double getPaidAmount() const;
    Date getPurchaseDate() const;
    Date getValidDate() const;
    TokenStatus getStatus() const;
    string getPurchaseTime() const;

    // Setters
    void setStatus(TokenStatus status);

    // Token operations
    bool isValid() const;
    bool isExpired() const;
    bool canBeUsed() const;
    void markAsUsed();
    void markAsReviewed();
    void displayToken() const;
    void saveToFile(const string& folderPath) const;


    // Static utility functions
    static string generateTokenNumber();
    static string tokenStatusToString(TokenStatus status);
};

// Meal review er class
class MealReview {
private:
    char Name[100];
    char tokenNumber[20];
    char mealName[100];
    MealRating rating;
    char comment[300];
    Date reviewDate;
    Halls hallName;
    int batch;
    department department;

public:
    // Constructors
    MealReview(const string& email, const string& tokenNum, const string& meal,
               MealRating rating, const string& comment, const string& date, const string& hall);
    MealReview();

    // Getters
    string getStudentEmail() const;
    string getTokenNumber() const;
    string getMealName() const;
    MealRating getRating() const;
    string getComment() const;
    string getReviewDate() const;
    string getHallName() const;

    // Display
    void displayReview() const;


    // Static utility functions
    static string ratingToString(MealRating rating);
};

class TokenManager {
private:
    vector<MealToken> activeTokens;
    vector<MealToken> usedTokens;
    vector<MealReview> reviews;
    static const string TOKEN_FOLDER;

public:
    // Constructor
    TokenManager();

    // Token operations
    string buyToken(const string& studentEmail, const string& hallName,
                   MealType mealType, const Meal& meal);
    bool useToken(const string& tokenNumber, const string& studentEmail);
    bool canBuyToken(const string& studentEmail, MealType mealType, const Date& forDate) const;
    vector<MealToken> getStudentTokens(const string& studentEmail) const;
    vector<MealToken> getActiveTokens() const { return activeTokens; }
    vector<MealToken> getUsedTokens() const { return usedTokens; }

    // Review er function gula
    bool addReview(const string& studentEmail, const string& tokenNumber,
                  MealRating rating, const string& comment);
    vector<MealReview> getMealReviews(const string& mealName) const;
    vector<MealReview> getHallReviews(const string& hallName) const;

    // File -> vector er kaj
    void saveAllTokens();
    void loadAllTokens();
    void cleanupExpiredTokens();

    // Review dis vallagena
    void saveAllReviews();
    void loadAllReviews();

    void displayStudentTokens(const string& studentEmail) const;
    void displayAllReviews() const;

private:
    void createTokenFolder();
    bool hasReviewedToken(const string& tokenNumber) const;
};


// Helper functiongula
class MealUtils {
public:
    static bool isWithinMealTime(MealType type);
    static string getCurrentTime();
    static bool isMealTimeExpired(MealType type, const Date& mealDate);
    static vector<string> getAvailableHalls();
    static void displayMealMenu(const vector<Meal>& meals);
    static void displayTokenReceipt(const MealToken& token);
};

#endif
