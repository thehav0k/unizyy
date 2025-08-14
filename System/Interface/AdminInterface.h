//
// Created by Md. Asif Khan on 11/8/25.
// Admin Interface for system administration
//

#ifndef ADMIN_INTERFACE_H
#define ADMIN_INTERFACE_H

#include "MenuInterface.h"
#include "../Users/Admin.h"

// Forward declaration
class Auth;

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
    void handleSystemSettings();
    void handleReports();
    void handleDatabaseManagement();
    void handleProfile();
    void handleLogout();
};

#endif //ADMIN_INTERFACE_H
