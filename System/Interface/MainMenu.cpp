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
    cout << "\nUNIZY : JAHANGIRNAGAR UNIVERSITY MANAGEMENT SYSTEM" << endl;
    cout << "==========================================================" << endl;
    cout << "Current Date: " << getCurrentTimeString() << endl;
    cout << "==========================================================" << endl;
}

void MainMenu::displayMenu() {
    displayWelcomeBanner();

    cout << "\nMAIN MENU OPTIONS:" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "0. Simulate Date & Time" << endl;
    cout << "1. Login to System" << endl;
    cout << "2. Register New Account" << endl;
    cout << "3. About System & Features" << endl;
    cout << "4. Exit Application" << endl;
    cout << "----------------------------------------------------------" << endl;
}

int MainMenu::getChoice() {
    int choice;
    string input;

    while (true) {
        cout << "Please enter your choice (0-4): ";
        getline(cin, input);

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
            case 1: { // Student registration with instant validation
                cout << "\n=== STUDENT REGISTRATION ===" << endl;
                cout << "Please provide the following information. You'll get instant feedback for each field.\n" << endl;

                // Use instant validation functions
                string studentID = Auth::getValidatedStudentID();
                string name = Auth::getValidatedName();
                string email = Auth::getValidatedEmail();
                string password = Auth::getValidatedPassword();

                int age;
                while (true) {
                    cout << "Enter Age (16-35): ";
                    cin >> age;
                    if (age >= 16 && age <= 35) {
                        cout << "[OK] Age is valid!" << endl;
                        break;
                    }
                    cout << "[ERROR] Age must be between 16 and 35. Please try again." << endl;
                }

                int classRoll;
                cout << "Enter Class Roll: ";
                cin >> classRoll;

                int deptChoice;
                cout << "Select Department:" << endl;
                cout << "1. Computer Science and Engineering" << endl;
                cout << "2. Physics" << endl;
                cout << "3. Mathematics" << endl;
                cout << "Enter choice (1-3): ";
                cin >> deptChoice;

                int batch;
                cout << "Enter Batch: ";
                cin >> batch;

                int hallChoice;
                cout << "Select Hall:" << endl;
                cout << "1. Al Beruni Hall" << endl;
                cout << "2. Meer Mosharraf Hossain Hall" << endl;
                cout << "3. Shaheed Salam Barkat Hall" << endl;
                cout << "4. AFM Kamaluddin Hall" << endl;
                cout << "5. Moulana Bhasani Hall" << endl;
                cout << "6. Bangabondhu Sheikh Majibur Rahman Hall" << endl;
                cout << "7. Jatiya Kabi Kazi Nazrul Islam Hall" << endl;
                cout << "8. Rabindra Nath Tagore Hall" << endl;
                cout << "9. Shahid Tajuddin Ahmed Hall" << endl;
                cout << "10. Shahid Sheikh Russel Hall" << endl;
                cout << "11. Shaheed Rafiq Jabbar Hall" << endl;
                cout << "12. Nawab Faizunnesa Hall" << endl;
                cout << "13. Fazilatunnesa Hall" << endl;
                cout << "14. Jahanara Imam Hall" << endl;
                cout << "15. Preetilata Hall" << endl;
                cout << "16. Begum Khaleda Zia Hall" << endl;
                cout << "17. Sheikh Hasina Hall" << endl;
                cout << "18. Bir Pratik Taramon Bibi Hall" << endl;
                cout << "Enter choice (1-18): ";
                cin >> hallChoice;
                cin.ignore();

                // Map dept and hall
                department dept = department::Department_of_Computer_Science_and_Engineering;
                if (deptChoice == 2) dept = department::Department_of_physics;
                else if (deptChoice == 3) dept = department::Department_of_Mathematics;

                Halls hall = Halls::Al_Beruni_Hall;
                switch (hallChoice) {
                    case 1: hall = Halls::Al_Beruni_Hall; break;
                    case 2: hall = Halls::Meer_Mosharraf_Hossain_Hall; break;
                    case 3: hall = Halls::Shaheed_Salam_Barkat_Hall; break;
                    case 4: hall = Halls::AFM_Kamaluddin_Hall; break;
                    case 5: hall = Halls::Moulana_Bhasani_Hall; break;
                    case 6: hall = Halls::Bangabondhu_Sheikh_Majibur_Rahman_Hall; break;
                    case 7: hall = Halls::Jatiya_Kabi_Kazi_Nazrul_Islam_Hall; break;
                    case 8: hall = Halls::Rabindra_Nath_Tagore_Hall; break;
                    case 9: hall = Halls::Shahid_Tajuddin_Ahmed_Hall; break;
                    case 10: hall = Halls::Shahid_Sheikh_Russel_Hall; break;
                    case 11: hall = Halls::Shaheed_Rafiq_Jabbar_Hall; break;
                    case 12: hall = Halls::Nawab_Faizunnesa_Hall; break;
                    case 13: hall = Halls::Fazilatunnesa_Hall; break;
                    case 14: hall = Halls::Jahanara_Imam_Hall; break;
                    case 15: hall = Halls::Preetilata_Hall; break;
                    case 16: hall = Halls::Begum_Khaleda_Zia_Hall; break;
                    case 17: hall = Halls::Sheikh_Hasina_Hall; break;
                    case 18: hall = Halls::Bir_Pratik_Taramon_Bibi_Hall; break;
                    default: hall = Halls::Al_Beruni_Hall; break;
                }

                success = authSystem->registerStudent(studentID, name, email, age, classRoll, dept, batch, hall, password);
                break;
            }
            case 2: { // Teacher registration with instant validation
                cout << "\n=== TEACHER REGISTRATION ===" << endl;
                cout << "Please provide the following information. You'll get instant feedback for each field.\n" << endl;

                // Use instant validation functions
                string name = Auth::getValidatedName();
                string email = Auth::getValidatedEmail();
                string password = Auth::getValidatedPassword();

                string dept;
                cout << "Enter Department: ";
                getline(cin, dept);

                int rankChoice;
                cout << "Select Academic Rank:" << endl;
                cout << "1. Professor" << endl;
                cout << "2. Associate Professor" << endl;
                cout << "3. Assistant Professor" << endl;
                cout << "4. Lecturer" << endl;
                cout << "Enter choice (1-4): ";
                cin >> rankChoice;
                cin.ignore();

                designation rank = designation::Professor;
                if (rankChoice == 2) rank = designation::AssociateProfessor;
                else if (rankChoice == 3) rank = designation::AssistantProfessor;
                else if (rankChoice == 4) rank = designation::Lecturer;

                success = authSystem->registerTeacher(name, email, dept, rank, password);
                break;
            }
            case 3: { // Admin registration with instant validation
                cout << "\n=== ADMIN REGISTRATION ===" << endl;
                cout << "Please provide the following information. You'll get instant feedback for each field.\n" << endl;

                // Use instant validation functions
                string name = Auth::getValidatedName();
                string email = Auth::getValidatedEmail();
                string password = Auth::getValidatedPassword();

                int typeChoice;
                cout << "Select Admin Type:" << endl;
                cout << "1. Transport" << endl;
                cout << "2. Hall Dining" << endl;
                cout << "3. Public Relations" << endl;
                cout << "4. Department" << endl;
                cout << "5. System Admin" << endl;
                cout << "Enter choice (1-5): ";
                cin >> typeChoice;
                cin.ignore();

                AdminType adminType = AdminType::Transport;
                if (typeChoice == 2) adminType = AdminType::HallDining;
                else if (typeChoice == 3) adminType = AdminType::PublicRelations;
                else if (typeChoice == 4) adminType = AdminType::Department;
                else if (typeChoice == 5) adminType = AdminType::SystemAdmin;

                success = authSystem->registerAdmin(name, email, adminType, password);
                break;
            }
            case 4: { // Dining Authority registration with instant validation
                cout << "\n=== DINING AUTHORITY REGISTRATION ===" << endl;
                cout << "Please provide the following information. You'll get instant feedback for each field.\n" << endl;

                // Use instant validation functions
                string name = Auth::getValidatedName();
                string email = Auth::getValidatedEmail();
                string password = Auth::getValidatedPassword();

                string hallName, designation, phone;
                cout << "Enter Assigned Hall Name: ";
                getline(cin, hallName);
                cout << "Enter Designation: ";
                getline(cin, designation);
                cout << "Enter Phone Number: ";
                getline(cin, phone);

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

    cout << "\nNew Registration Experience:" << endl;
    cout << "   • Instant email validation feedback" << endl;
    cout << "   • Real-time password requirement checking" << endl;
    cout << "   • Immediate duplicate email/ID detection" << endl;
    cout << "   • Clear error messages and requirements" << endl;

    pauseForInput();
}

void MainMenu::handleExit() {
    displayHeader("EXIT SYSTEM");
    cout << "Thank you for using the University Management System!" << endl;
    cout << "Have a great day!" << endl;
    isRunning = false;
}

void MainMenu::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}
