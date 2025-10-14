
#ifndef STUDENT_INTERFACE_H
#define STUDENT_INTERFACE_H

#include "MenuInterface.h"
#include "../Users/student.h"
#include "../Modules/Meal/meal.h"

// Forward declaration
class Auth;

class StudentInterface : public MenuInterface {
private:
    Student* currentStudent;  //Ei student er current login info
    Auth* authSystem;        //Authentication system er jonno pointer
    TokenManager* tokenManager;  //Meal token system
    bool isRunning;                 //Interface cholche kina tar status(true or false)
public:
    //Constructor
    StudentInterface(Student* student, Auth* auth);
    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override;
    void run() override;

private:
    //Dashboard and menu related
    void displayStudentDashboard();
    void handleMealTokenOperations();
    void handleBuyToken();
    void handleUseToken();
    void handleViewMyTokens();
    void handleReviewMeal();
    void handleViewProfile();
    void handleChangePassword();
    void handleLogout();


    void displayAvailableHalls();
    void displayMealTypes();
    void displayMealDetailsForPurchase(const string& hallName, MealType mealType);
    Meal selectMealFromMenu(const vector<Meal>& meals);

    //Notices
    void handleViewNotices();
};

#endif //STUDENT_INTERFACE_H
