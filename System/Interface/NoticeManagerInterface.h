
#ifndef NOTICEMANAGERINTERFACE_H
#define NOTICEMANAGERINTERFACE_H

#include"MenuInterface.h"
#include <vector>
#include "NoticeManager.h"
#include "../Authentication/auth.h"


using namespace std;
class NoticeInterface:public MenuInterface {
private:
    Auth* authSystem;
    std::vector<Notice>& notices;
    bool isRunning;

    void displayNoticeDashboard();
    void displayMenu(bool isAdmin);
    int getChoice(int maxChoice);
    void processChoice(int choice, bool isAdmin);


    void handleCreateNotice();
    void handleUpdateNotice();
    void handleViewNotices();

    void handleLogout();

public:
    NoticeInterface(Auth* auth, std::vector<Notice>& noticeBoard);
    void run();
};


#endif //NOTICEMANAGERINTERFACE_H
