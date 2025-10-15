#ifndef PUBLICRELATIONSADMININTERFACE_H
#define PUBLICRELATIONSADMININTERFACE_H

#include "MenuInterface.h"
#include "../Users/PublicRelationsAdmin.h"

// Forward declaration
class Auth;

class PublicRelationsAdminInterface : public MenuInterface {
private:
    PublicRelationsAdmin* currentAdmin; //Ei admin er current login info
    Auth* authSystem;   //Authentication system er jonno pointer
    bool isRunning;       //Interface cholche kina tar status(true or false)

public:
    PublicRelationsAdminInterface(PublicRelationsAdmin* admin, Auth* auth);
    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override;
    void run() override;

private:
    void displayAdminDashboard();
    //Notice operations
    void handleCreateNotice();
    void handleViewAllNotices();
    void handleDeleteNotice();
    void handleViewNoticeDetails();
    void handleNoticeStatistics();
    void handleViewProfile();
    void handleChangePassword();
    void handleLogout();
};

#endif // PUBLICRELATIONSADMININTERFACE_H
