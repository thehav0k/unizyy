#ifndef DINING_AUTHORITY_INTERFACE_H
#define DINING_AUTHORITY_INTERFACE_H

#include "MenuInterface.h"
#include "../Users/DiningAuthority.h"
#include "../Modules/Meal/meal.h"

// Forward declaration
class Auth;

class DiningAuthorityInterface : public MenuInterface {
private:
    DiningAuthority* currentAuthority;
    Auth* authSystem;
    TokenManager* tokenManager; // may be unused now
    bool isRunning;

public:
    DiningAuthorityInterface(DiningAuthority* authority, Auth* auth);

    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override;
    void run() override;

private:
    void displayAuthorityDashboard();
    void handleMealManagement();
    void handleCreateMeal();
    void handleUpdateMeal();
    void handleViewReviews();
    void handleAddNotice();
    void handleProfile();
    void handleLogout();
};

#endif //DINING_AUTHORITY_INTERFACE_H
