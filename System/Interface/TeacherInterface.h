//
// Created by Md. Asif Khan on 11/8/25.
// Teacher Interface for teacher operations
//

#ifndef TEACHER_INTERFACE_H
#define TEACHER_INTERFACE_H

#include "MenuInterface.h"
#include "../Users/Teacher.h"

// Forward declaration
class Auth;

class TeacherInterface : public MenuInterface {
private:
    Teacher* currentTeacher;
    Auth* authSystem;
    bool isRunning;

public:
    TeacherInterface(Teacher* teacher, Auth* auth);

    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override;
    void run() override;

private:
    void displayTeacherDashboard();
    void handleCourseManagement();
    void handleStudentGrading();
    void handleAttendance();
    void handleNoticeManagement();
    void handleProfile();
    void handleLogout();

    // Notice-specific methods
    void createNotice();
    void viewMyNotices();
    void viewAllNotices();
    void searchNotices();
    void editMyNotice();
    void deleteMyNotice();
    void displayNoticesList(const std::vector<Notice>& notices);
    NoticeType selectNoticeType();
};

#endif //TEACHER_INTERFACE_H
