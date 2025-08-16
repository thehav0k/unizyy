//
// Created by Md. Asif Khan on 15/8/25.
// Public Relations Admin Interface for notice management
//

#ifndef PUBLICRELATIONSADMIN_INTERFACE_H
#define PUBLICRELATIONSADMIN_INTERFACE_H

#include "MenuInterface.h"
#include "../Users/PublicRelationsAdmin.h"

// Forward declaration
class Auth;

class PublicRelationsAdminInterface : public MenuInterface {
private:
    PublicRelationsAdmin* currentPRAdmin;
    Auth* authSystem;
    bool isRunning;

public:
    PublicRelationsAdminInterface(PublicRelationsAdmin* prAdmin, Auth* auth);

    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override;
    void run() override;

private:
    void displayPRAdminDashboard();
    void handleNoticeCreation();
    void handleNoticeManagement();
    void handleNoticeSearch();
    void handleNoticeStatistics();
    void handleBulkOperations();
    void handleProfile();
    void handleLogout();

    // Notice-specific helper methods
    void createSimpleNotice();
    void createDetailedNotice();
    void viewAllNotices();
    void viewMyNotices();
    void editNotice();
    void deleteNotice();
    void searchNotices();
    void filterNoticesByType();
    void displayNoticesByType(NoticeType type);
    void publishMultipleNotices();
    void archiveOldNotices();
    NoticeType selectNoticeType();
    void displayNoticeDetails(const Notice& notice);
    void displayNoticesList(const std::vector<Notice>& notices);
};

#endif //PUBLICRELATIONSADMIN_INTERFACE_H
