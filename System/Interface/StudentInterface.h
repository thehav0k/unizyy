//
// Created by Md. Asif Khan on 11/8/25.
// Student Interface for meal token management and student operations
//

#ifndef STUDENT_INTERFACE_H
#define STUDENT_INTERFACE_H

#include "MenuInterface.h"
#include "../Users/student.h"
#include "../Modules/Meal/meal.h"
#include "../../Core/Models/Notice.h"

// Forward declaration
class Auth;

class StudentInterface : public MenuInterface {
private:
    Student* currentStudent;
    Auth* authSystem;
    TokenManager* tokenManager;
    bool isRunning;

public:
    StudentInterface(Student* student, Auth* auth);

    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override;
    void run() override;

private:
    void displayStudentDashboard();
    void handleMealTokenOperations();
    void handleBuyToken();
    void handleUseToken();
    void handleViewMyTokens();
    void handleReviewMeal();
    void handleViewProfile();
    void handleChangePassword();
    void handleViewNotices();
    void handleLogout();

    // Meal token specific UI methods
    void displayAvailableHalls();
    void displayMealTypes();
    void displayMealDetailsForPurchase(const string& hallName, MealType mealType);
    Meal selectMealFromMenu(const vector<Meal>& meals);

    // Notice viewing methods
    void viewAllActiveNotices();
    void searchNotices();
    void filterNoticesByType();
    void viewNoticeDetails();
    void displayNoticesList(const std::vector<Notice>& notices);
};

#endif //STUDENT_INTERFACE_H
