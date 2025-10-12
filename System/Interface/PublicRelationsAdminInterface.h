//
// Created by Md. Asif Khan on 12/10/25.
//

#ifndef PUBLICRELATIONSADMININTERFACE_H
#define PUBLICRELATIONSADMININTERFACE_H

#include "MenuInterface.h"
#include "../Users/PublicRelationsAdmin.h"

// Forward declaration
class Auth;

class PublicRelationsAdminInterface : public MenuInterface {
private:
    PublicRelationsAdmin* currentAdmin;
    Auth* authSystem;
    bool isRunning;

public:
    PublicRelationsAdminInterface(PublicRelationsAdmin* admin, Auth* auth);
    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override;
    void run() override;

private:
    void displayAdminDashboard();
    void handleCreateNotice();
    void handleViewAllNotices();
    void handleEditNotice();
    void handleDeleteNotice();
    void handleViewNoticeDetails();
    void handleSearchNotices();
    void handleNoticeStatistics();
    void handleViewProfile();
    void handleChangePassword();
    void handleLogout();
};

#endif // PUBLICRELATIONSADMININTERFACE_H

