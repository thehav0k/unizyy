#ifndef ADMIN_INTERFACE_H
#define ADMIN_INTERFACE_H

#include "MenuInterface.h"
#include "../Users/Admin.h"
// Forward declaration
class Auth;
//ekhane inheritance use kora hoyeche (4)
//AdminInterface class MenuInterface theke derive kora
class AdminInterface : public MenuInterface {
private:
    Admin* currentAdmin;
    Auth* authSystem;
    bool isRunning;

public:
    AdminInterface(Admin* admin, Auth* auth);

    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override;
    void run() override;

private:
    void displayAdminDashboard();
    void handleUserManagement();
    void handleViewAllUsers();
    void handleAddNewUser();
    void handleSearchUser();
    void handleUserStatistics();
    void handleNoticeManagement();
    void handleProfile();
    void handleLogout();
};
void handleNoticeManagement();

#endif //ADMIN_INTERFACE_H
