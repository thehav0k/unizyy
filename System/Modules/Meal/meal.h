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
    ACTIVE = 0,      // Token is valid and unused
    USED = 1,        // Token has been used to get meal
    EXPIRED = 2,     // Token has expired
    REVIEWED = 3     // Token has been used and reviewed // then the token is to be deleted
};

class Meal {
private:
    char MealPackage[300]; // it will have three packages for each mealtype like (Rice, Chicken Curry, Lentils) / (Rice, Fish, Vegetables) etc
    MealType mealType;
    double price; // price in bdt
    int availableQuantity;
    bool isAvailable;
    // Use char array to store date string directly (DD-MM-YYYY format)
    char date[12]; // "DD-MM-YYYY" + null terminator
    char time[10]; // for recording buying time
    Halls hallName;

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

    // Binary file operations
    void writeToBinaryFile(ofstream& out) const;
    void readFromBinaryFile(ifstream& in);

    // Static utility functions
    static string mealTypeToString(MealType type);
    static MealType stringToMealType(const string& typeStr);

    // Static database management functions - using Database folder
    static vector<Meal> loadAllMealsFromDatabase();
    static vector<Meal> loadMealsByHall(const string& hallName);
    static vector<Meal> loadMealsByDate(const string& date);
    static vector<Meal> loadMealsByType(MealType type);
    static bool deleteMealFromDatabase(const string& date, const string& hallName, MealType type);
    static void initializeMealDatabase();
    static void displayAllMeals();
    static size_t getMealCount();

    // Instance method for saving current meal
    bool saveMealToDatabase() const;
    bool writeDirectlyToFile(const string& filename) const;
    bool readDirectlyFromFile(const string& filename);
    static bool writeDirectlyToDatabase();
    static vector<Meal> loadDirectlyFromDatabase();
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
    bool canBeUsed() const; // new helper
    void markAsUsed();
    void markAsReviewed();
    void displayToken() const;
    void saveToFile(const string& folderPath) const;

    // Binary file operations
    void writeToBinaryFile(ofstream& out) const;
    void readFromBinaryFile(ifstream& in);

    // Static utility functions
    static string generateTokenNumber();
    static string tokenStatusToString(TokenStatus status);
};

class MealReview {
private:
    char Name[100];          // Student name
    char tokenNumber[20];     // Token used for this meal
    char mealName[100];       // Meal name for this review
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

    // Binary file operations
    void writeToBinaryFile(ofstream& out) const;
    void readFromBinaryFile(ifstream& in);

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

    // Review operations
    bool addReview(const string& studentEmail, const string& tokenNumber,
                  MealRating rating, const string& comment);
    vector<MealReview> getMealReviews(const string& mealName) const;
    vector<MealReview> getHallReviews(const string& hallName) const;

    // File operations
    void saveAllTokens();
    void loadAllTokens();
    void cleanupExpiredTokens();

    // Display operations
    void displayStudentTokens(const string& studentEmail) const;
    void displayAllReviews() const;

private:
    void createTokenFolder();
    bool hasReviewedToken(const string& tokenNumber) const;
};


// Utility functions
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
