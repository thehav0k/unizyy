#ifndef TEACHER_INTERFACE_H
#define TEACHER_INTERFACE_H

#include "MenuInterface.h"
#include "../Users/Teacher.h"

//Forward declaration kora hoyeche
class Auth;

class TeacherInterface : public MenuInterface {
private:
    Teacher* currentTeacher;  //Ei teacher er current login info (pointer use kora hoyeche)
    Auth* authSystem;    //Authentication system er jonno pointer (pointer use kora hoyeche)
    bool isRunning;   //Interface cholche kina tar status(true or false)

public:
    //Constructor
    TeacherInterface(Teacher* teacher, Auth* auth);

    void displayMenu() override;
    int getChoice() override;
    void processChoice(int choice) override; //Choice onujayi operation process kore(runtime polymorpshism)
    void run() override;  //Main interface loop run kore

private:
    void displayTeacherDashboard();
    void handleProfile();
    void handleLogout();
};

#endif //TEACHER_INTERFACE_H
