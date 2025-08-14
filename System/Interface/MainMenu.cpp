//
// Created by Md. Asif Khan on 11/8/25.
// Main Menu Implementation
//

#include "MainMenu.h"
#include "../Authentication/auth.h"
#include "StudentInterface.h"
#include "AdminInterface.h"
#include "TeacherInterface.h"
#include "DiningAuthorityInterface.h"
#include <iostream>
#include <limits>
#include "../../Core/Models/date.h"  // for date simulation

using namespace std;

MainMenu::MainMenu(Auth* auth) : authSystem(auth), isRunning(true) {}

void MainMenu::displayWelcomeBanner() {
    clearScreen();
    cout << "\n🏛️  UNIVERSITY MANAGEMENT SYSTEM 2025" << endl;
    cout << "═══════════════════════════════════════════════════════════" << endl;
    cout << "🎓 Comprehensive University Management Solution" << endl;
    cout << "👥 Student | 👨‍🏫 Teacher | 🔧 Admin | 🍽️  Dining Management" << endl;
    cout << "📅 Current Date: " << getCurrentTimeString() << endl;
    cout << "═══════════════════════════════════════════════════════════" << endl;
}

void MainMenu::displayMenu() {
    displayWelcomeBanner();

    cout << "\n🚀 MAIN MENU OPTIONS:" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "⏰ 0. Simulate Date & Time" << endl;
    cout << "🔐 1. Login to System" << endl;
    cout << "📝 2. Register New Account" << endl;
    cout << "ℹ️  3. About System & Features" << endl;
    cout << "🚪 4. Exit Application" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
}

int MainMenu::getChoice() {
    int choice;
    string input;

    while (true) {
        cout << "Please enter your choice (0-4): ";
        getline(cin, input);

        // Check if input is empty
        if (input.empty()) {
            displayError("Please enter a number between 0-4.");
            continue;
        }

        // Try to convert to integer
        try {
            choice = stoi(input);
            if (choice >= 0 && choice <= 4) {
                return choice;
            } else {
                displayError("Invalid input! Please enter a number between 0-4.");
            }
        } catch (const exception&) {
            displayError("Invalid input! Please enter a number between 0-4.");
        }
    }
}

void MainMenu::processChoice(int choice) {
    switch (choice) {
        case 0:
            // simulate date
            {
                displayHeader("SIMULATE DATE & TIME");
                cout << "1. Set specific date (DD-MM-YYYY)" << endl;
                cout << "2. Advance N days" << endl;
                cout << "3. Advance N months" << endl;
                cout << "4. Advance N hours" << endl;
                cout << "5. Set date + hour" << endl;
                cout << "6. Reset simulation" << endl;
                cout << "Enter choice (1-6): ";
                int simChoice; string input; getline(cin, input); if(input.empty()) simChoice=0; else simChoice = stoi(input);
                switch (simChoice) {
                    case 1: {
                        cout << "Enter date (DD-MM-YYYY): "; string d; getline(cin, d);
                        if (d.empty()) displayError("Empty date"); else { Date dt(d); if(!dt.isValid()) displayError("Invalid date"); else { Date::setSimulatedDate(dt); displaySuccess("Simulation date set to " + dt.toString()); } }
                    } break;
                    case 2: {
                        cout << "Enter days to advance: "; size_t n; cin >> n; cin.ignore(); Date dt = Date::SimulateDate(n); displaySuccess("Advanced to " + dt.toString()); }
                        break;
                    case 3: {
                        cout << "Enter months to advance: "; size_t n; cin >> n; cin.ignore(); Date dt = Date::SimulateMonths(n); displaySuccess("Advanced to " + dt.toString()); }
                        break;
                    case 4: {
                        cout << "Enter hours to advance: "; size_t n; cin >> n; cin.ignore(); Date::SimulateHours(n); Date cur = Date::getCurrentDate(); int h = Date::getSimulatedHour(); displaySuccess("Time advanced. Now: " + cur.toString() + " " + to_string(h) + ":00"); }
                        break;
                    case 5: {
                        cout << "Enter date (DD-MM-YYYY): "; string d; getline(cin, d); cout << "Enter hour (0-23): "; int h; cin >> h; cin.ignore(); Date dt(d); if(!dt.isValid()) displayError("Invalid date"); else { Date::setSimulatedDateTime(dt,h); displaySuccess("Set to " + dt.toString() + " " + to_string(Date::getSimulatedHour()) + ":00"); } }
                        break;
                    case 6: { Date::resetSimulation(); displaySuccess("Simulation reset."); } break;
                    default: displayInfo("Cancelled.");
                }
                pauseForInput();
            }
            break;
        case 1:
            handleLogin();
            break;
        case 2:
            handleRegistration();
            break;
        case 3:
            handleAbout();
            break;
        case 4:
            handleExit();
            break;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void MainMenu::handleLogin() {
    displayHeader("LOGIN SYSTEM");

    string email, password;
    cout << "Enter email: ";
    getline(cin, email);
    cout << "Enter password: ";
    getline(cin, password);

    displayInfo("Authenticating user...");

    User* user = authSystem->login(email, password);
    if (user) {
        displaySuccess("Login successful!");
        cout << "Welcome, " << email << "!" << endl;

        // Determine user type and launch appropriate interface
        if (auto* student = dynamic_cast<Student*>(user)) {
            StudentInterface studentUI(student, authSystem);
            studentUI.run();
        }
        else if (auto* admin = dynamic_cast<Admin*>(user)) {
            AdminInterface adminUI(admin, authSystem);
            adminUI.run();
        }
        else if (auto* teacher = dynamic_cast<Teacher*>(user)) {
            TeacherInterface teacherUI(teacher, authSystem);
            teacherUI.run();
        }
        else if (auto* diningAuth = dynamic_cast<DiningAuthority*>(user)) {
            DiningAuthorityInterface diningUI(diningAuth, authSystem);
            diningUI.run();
        }
    } else {
        displayError("Invalid credentials! Please try again.");
        pauseForInput();
    }
}

void MainMenu::handleRegistration() {
    displayHeader("REGISTRATION SYSTEM");

    cout << "Select account type:" << endl;
    cout << "1. Student" << endl;
    cout << "2. Teacher" << endl;
    cout << "3. Admin" << endl;
    cout << "4. Dining Authority" << endl;
    cout << "5. Back to Main Menu" << endl;

    int userType;
    cout << "Enter choice (1-5): ";
    cin >> userType;
    cin.ignore();

    if (userType == 5) return;

    if (userType >= 1 && userType <= 4) {
        bool success = false;
        switch (userType) {
            case 1: { // Student registration
                string studentID, name, email, password;
                int age, classRoll, batch, hallChoice, deptChoice;
                cout << "Enter Student ID: "; getline(cin, studentID);
                cout << "Enter Name: "; getline(cin, name);
                cout << "Enter Email: "; getline(cin, email);
                cout << "Enter Age: "; cin >> age;
                cout << "Enter Class Roll: "; cin >> classRoll;
                cout << "Select Department (1:CSE,2:Physics,3:Mathematics): "; cin >> deptChoice;
                cout << "Enter Batch: "; cin >> batch;
                cout << "Select Hall (1:Al Beruni,2:Meer Mosharraf,3:Shaheed Salam Barkat,4:AFM Kamaluddin,5:Moulana Bhasani,6:Bangabondhu Sheikh Majibur Rahman): "; cin >> hallChoice;
                cin.ignore();
                cout << "Enter Password: "; getline(cin, password);
                // Map dept and hall
                department dept = department::Department_of_Computer_Science_and_Engineering;
                if (deptChoice == 2) dept = department::Department_of_physics;
                else if (deptChoice == 3) dept = department::Department_of_Mathematics;
                Halls hall = Halls::Al_Beruni_Hall;
                switch (hallChoice) {
                    case 2: hall = Halls::Meer_Mosharraf_Hossain_Hall; break;
                    case 3: hall = Halls::Shaheed_Salam_Barkat_Hall; break;
                    case 4: hall = Halls::AFM_Kamaluddin_Hall; break;
                    case 5: hall = Halls::Moulana_Bhasani_Hall; break;
                    case 6: hall = Halls::Bangabondhu_Sheikh_Majibur_Rahman_Hall; break;
                }
                success = authSystem->registerStudent(studentID, name, email, age, classRoll, dept, batch, hall, password);
                break;
            }
            case 2: { // Teacher registration
                string name, email, dept, password;
                int rankChoice;
                cout << "Enter Name: "; getline(cin, name);
                cout << "Enter Email: "; getline(cin, email);
                cout << "Enter Department: "; getline(cin, dept);
                cout << "Select Rank (1:Professor,2:AssociateProfessor,3:AssistantProfessor,4:Lecturer): "; cin >> rankChoice;
                cin.ignore();
                cout << "Enter Password: "; getline(cin, password);
                AcademicPosition rank = AcademicPosition::Professor;
                if (rankChoice == 2) rank = AcademicPosition::AssociateProfessor;
                else if (rankChoice == 3) rank = AcademicPosition::AssistantProfessor;
                else if (rankChoice == 4) rank = AcademicPosition::Lecturer;
                success = authSystem->registerTeacher(name, email, dept, rank, password);
                break;
            }
            case 3: { // Admin registration
                string name, email, password;
                int typeChoice;
                cout << "Enter Name: "; getline(cin, name);
                cout << "Enter Email: "; getline(cin, email);
                cout << "Select Admin Type (1:Transport,2:HallDining,3:PublicRelations,4:Department,5:SystemAdmin): "; cin >> typeChoice;
                cin.ignore();
                cout << "Enter Password: "; getline(cin, password);
                AdminType adminType = AdminType::Transport;
                if (typeChoice == 2) adminType = AdminType::HallDining;
                else if (typeChoice == 3) adminType = AdminType::PublicRelations;
                else if (typeChoice == 4) adminType = AdminType::Department;
                else if (typeChoice == 5) adminType = AdminType::SystemAdmin;
                success = authSystem->registerAdmin(name, email, adminType, password);
                break;
            }
            case 4: { // Dining Authority registration
                string name, email, hallName, designation, phone, password;
                cout << "Enter Name: "; getline(cin, name);
                cout << "Enter Email: "; getline(cin, email);
                cout << "Enter Assigned Hall Name: "; getline(cin, hallName);
                cout << "Enter Designation: "; getline(cin, designation);
                cout << "Enter Phone Number: "; getline(cin, phone);
                cout << "Enter Password: "; getline(cin, password);
                success = authSystem->registerDiningAuthority(name, email, hallName, designation, phone, password);
                break;
            }
        }
        if (success) displaySuccess("Registration successful!");
        else displayError("Registration failed. Please try again.");
    } else {
        displayError("Invalid selection!");
    }

    pauseForInput();
}

void MainMenu::handleAbout() {
    displayHeader("ABOUT UNIVERSITY MANAGEMENT SYSTEM");

    cout << "System Features:" << endl;
    cout << "   • Student Management" << endl;
    cout << "   • Teacher Management" << endl;
    cout << "   • Administrative Functions" << endl;
    cout << "   • Dining Hall Management" << endl;
    cout << "   • Meal Token System" << endl;
    cout << "   • Analytics & Reports" << endl;
    cout << "   • Secure Authentication" << endl;

    cout << "\nMeal Token System:" << endl;
    cout << "   • Buy tokens day before meals" << endl;
    cout << "   • Maximum 1 token per meal type per day" << endl;
    cout << "   • Time-based validation system" << endl;
    cout << "   • Review system after meals" << endl;
    cout << "   • Automatic token expiration" << endl;

    cout << "\nSupported Halls:" << endl;
    cout << "   • Al Beruni Hall" << endl;
    cout << "   • Meer Mosharraf Hossain Hall" << endl;
    cout << "   • Shaheed Salam Barkat Hall" << endl;
    cout << "   • And many more..." << endl;

    cout << "\nDeveloped by: Md. Asif Khan" << endl;
    cout << "Version: 1.0.0 (August 2025)" << endl;

    pauseForInput();
}

void MainMenu::handleExit() {
    displayHeader("EXIT CONFIRMATION");

    if (confirmAction("Are you sure you want to exit?")) {
        displayInfo("Thank you for using University Management System!");
        cout << "Have a great day!" << endl;
        isRunning = false;
    }
}

void MainMenu::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}
