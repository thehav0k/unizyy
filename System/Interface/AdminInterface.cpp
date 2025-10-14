#include "AdminInterface.h"
#include "../Authentication/auth.h"
#include <iostream>
#include <limits>
#include "../../Core/Models/date.h"
#include "../../Core/Models/Notice.h"
#include "../../Core/Models/department.h"

using namespace std;

AdminInterface::AdminInterface(Admin* admin, Auth* auth)
    : currentAdmin(admin), authSystem(auth), isRunning(true) {}

void AdminInterface::displayAdminDashboard() {
    clearScreen();
    displayHeader("ADMIN DASHBOARD");
    cout << "Welcome, " << currentAdmin->getName() << "!" << endl;
    cout << "Admin Type: " << toString(currentAdmin->getAdminType()) << endl;
    cout << "Email: " << currentAdmin->getEmail() << endl;
    displaySeparator('-', 60);
}

void AdminInterface::displayMenu() {
    displayAdminDashboard();

    cout << "\nADMIN MENU OPTIONS:" << endl;
    cout << "0. Simulate Date" << endl;

    cout << "1. User Management" << endl;
    cout << "2. System Settings" << endl;
    cout << "3. System Reports" << endl;
    cout << "4. Notice Management(create,update,view)" << endl;
    cout << "5. Database Management" << endl;
    cout << "6. Profile Settings" << endl;
    cout << "7. Logout" << endl;
    displaySeparator('-', 40);
}

int AdminInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (0-7): ";

    while (!(cin >> choice) || choice < 0 || choice > 7) {
        displayError("Invalid input! Please enter a number between 0-6.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (0-7): ";
    }

    cin.ignore();
    return choice;
}

void AdminInterface::processChoice(int choice) {
    switch (choice) {
        case 0: {
            displayHeader("SIMULATE DATE");
            cout << "Enter date (DD-MM-YYYY) or leave empty to reset: ";
            string input;
            getline(cin, input);
            if (input.empty()) { Date::resetSimulation(); displaySuccess("Simulation reset to current date."); }
            else { Date d(input); if (!d.isValid()) displayError("Invalid date format."); else { Date::setSimulatedDate(d); displaySuccess("Simulation date set to " + d.toString()); }}
            pauseForInput();
        } break;
        case 1:
            handleUserManagement();
            break;
        case 2:
            handleSystemSettings();
            break;
        case 3:
            handleReports();
            break;
        case 4:
            handleNoticeManagement();
            break;
        case 5:
            handleDatabaseManagement();
            break;
        case 6:
            handleProfile();
            break;
        case 7:
            handleLogout();
            break;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void AdminInterface::handleUserManagement() {
    while (true) {
        clearScreen();
        displayHeader("USER MANAGEMENT");

        cout << "USER MANAGEMENT OPTIONS:" << endl;
        cout << "1. View All Users" << endl;
        cout << "2. Add New User" << endl;
        cout << "3. Search User by Email" << endl;
        cout << "4. User Statistics" << endl;
        cout << "5. Deactivate/Activate User" << endl;
        cout << "6. Back to Admin Menu" << endl;
        cout << "----------------------------------------" << endl;

        cout << "Please enter your choice (1-6): ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                handleViewAllUsers();
                break;
            case 2:
                handleAddNewUser();
                break;
            case 3:
                handleSearchUser();
                break;
            case 4:
                handleUserStatistics();
                break;
            case 5:
                handleUserActivation();
                break;
            case 6:
                return; // Back to admin menu
            default:
                displayError("Invalid choice! Please enter 1-6.");
                pauseForInput();
        }
    }
}

void AdminInterface::handleViewAllUsers() {
    clearScreen();
    displayHeader("ALL USERS");

    // Load and display students
    vector<Student> students = DatabaseManager::loadStudents();
    if (!students.empty()) {
        cout << "\n=== STUDENTS ===" << endl;
        for (size_t i = 0; i < students.size(); i++) {
            cout << i+1 << ". " << students[i].getName()
                 << " (" << students[i].getEmail() << ")"
                 << " - ID: " << students[i].getStudentID()
                    << " - Dept: " << toString(students[i].getDepartment()) << endl;
        }
    }

    // Load and display teachers
    vector<Teacher> teachers = DatabaseManager::loadTeachers();
    if (!teachers.empty()) {
        cout << "\n=== TEACHERS ===" << endl;
        for (size_t i = 0; i < teachers.size(); i++) {
            cout << i+1 << ". " << teachers[i].getName()
                 << " (" << teachers[i].getEmail() << ")"
                    << " - Dept: " << toString(teachers[i].getDepartment()) << endl;
        }
    }

    // Load and display admins
    vector<Admin> admins = DatabaseManager::loadAdmins();
    if (!admins.empty()) {
        cout << "\n=== ADMINS ===" << endl;
        for (size_t i = 0; i < admins.size(); i++) {
            cout << i+1 << ". " << admins[i].getName()
                 << " (" << admins[i].getEmail() << ")"
                 << " - Type: " << toString(admins[i].getAdminType()) << endl;
        }
    }

    // Load and display dining authorities
    vector<DiningAuthority> dining = DatabaseManager::loadDiningAuthorities();
    if (!dining.empty()) {
        cout << "\n=== DINING AUTHORITIES ===" << endl;
        for (size_t i = 0; i < dining.size(); i++) {
            cout << i+1 << ". " << dining[i].getName()
                 << " (" << dining[i].getEmail() << ")"
                 << " - Hall: " << dining[i].getHallName() << endl;
        }
    }

    pauseForInput();
}

void AdminInterface::handleAddNewUser() {
    clearScreen();
    displayHeader("ADD NEW USER");

    cout << "Select user type to add:" << endl;
    cout << "1. Student" << endl;
    cout << "2. Teacher" << endl;
    cout << "3. Admin" << endl;
    cout << "4. Dining Authority" << endl;
    cout << "5. Cancel" << endl;

    int userType;
    cout << "Enter choice (1-5): ";
    cin >> userType;
    cin.ignore();

    if (userType == 5) return;

    bool success = false;
    switch (userType) {
        case 1: {
            displayInfo("Adding new student...");
            // Use the same registration logic from MainMenu
            string studentID = Auth::getValidatedStudentID();
            string name = Auth::getValidatedName();
            string email = Auth::getValidatedEmail();
            string password = Auth::getValidatedPassword();

            // Get other required fields (simplified for admin)
            int age = 20; // Default or get from admin
            int classRoll;
            cout << "Enter Class Roll: ";
            cin >> classRoll;
            cin.ignore();

            // Simplified - use defaults or extend as needed
            department dept = department::Department_of_Computer_Science_and_Engineering;
            int batch = 50;
            Halls hall = Halls::Al_Beruni_Hall;

            success = authSystem->registerStudent(studentID, name, email, age, classRoll, dept, batch, hall, password);
            break;
        }
        case 2: {
            displayInfo("Adding new teacher...");
            string name = Auth::getValidatedName();
            string email = Auth::getValidatedEmail();
            string password = Auth::getValidatedPassword();

            int dept;
            cout << "Select Department:" << endl;
            cout << "1. Computer Science and Engineering" << endl;
            cout << "2. Physics" << endl;
            cout << "3. Mathematics" << endl;
            cout << "Enter choice (1-3): ";
            cin >> dept;

            cin.ignore();

            department deptchoice = department::Department_of_Computer_Science_and_Engineering;
            if (dept == 2) deptchoice = department::Department_of_physics;
            else if (dept == 3) deptchoice = department::Department_of_Mathematics;

            designation des = designation::Lecturer; // Default

            success = authSystem->registerTeacher(name, email, deptchoice, des, password);
            break;
        }
        case 3: {
            displayInfo("Adding new admin...");
            string name = Auth::getValidatedName();
            string email = Auth::getValidatedEmail();
            string password = Auth::getValidatedPassword();

            AdminType adminType = AdminType::SystemAdmin; // Default

            success = authSystem->registerAdmin(name, email, adminType, password);
            break;
        }
        case 4: {
            displayInfo("Adding new dining authority...");
            string name = Auth::getValidatedName();
            string email = Auth::getValidatedEmail();
            string password = Auth::getValidatedPassword();

            int hallChoice;
            cout << "Select Assigned Hall:" << endl;
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

            string hallName = hallToString(hall);
            success = authSystem->registerDiningAuthority(name, email, hallName, password);
            break;
        }
    }

    if (success) {
        displaySuccess("User added successfully!");
    } else {
        displayError("Failed to add user. Please try again.");
    }

    pauseForInput();
}

void AdminInterface::handleSearchUser() {
    clearScreen();
    displayHeader("SEARCH USER");

    cout << "Enter email to search: ";
    string email;
    getline(cin, email);

    bool found = false;

    // Search in students
    vector<Student> students = DatabaseManager::loadStudents();
    for (const auto& student : students) {
        if (student.getEmail() == email) {
            cout << "\n=== STUDENT FOUND ===" << endl;
            student.display();
            found = true;
            break;
        }
    }

    // Search in teachers if not found
    if (!found) {
        vector<Teacher> teachers = DatabaseManager::loadTeachers();
        for (const auto& teacher : teachers) {
            if (teacher.getEmail() == email) {
                cout << "\n=== TEACHER FOUND ===" << endl;
                teacher.display();
                found = true;
                break;
            }
        }
    }

    // Search in admins if not found
    if (!found) {
        vector<Admin> admins = DatabaseManager::loadAdmins();
        for (const auto& admin : admins) {
            if (admin.getEmail() == email) {
                cout << "\n=== ADMIN FOUND ===" << endl;
                admin.display();
                found = true;
                break;
            }
        }
    }

    // Search in dining authorities if not found
    if (!found) {
        vector<DiningAuthority> dining = DatabaseManager::loadDiningAuthorities();
        for (const auto& auth : dining) {
            if (auth.getEmail() == email) {
                cout << "\n=== DINING AUTHORITY FOUND ===" << endl;
                auth.display();
                found = true;
                break;
            }
        }
    }

    if (!found) {
        displayError("User not found with email: " + email);
    }

    pauseForInput();
}

void AdminInterface::handleUserStatistics() {
    clearScreen();
    displayHeader("USER STATISTICS");

    int studentCount = DatabaseManager::loadStudents().size();
    int teacherCount = DatabaseManager::loadTeachers().size();
    int adminCount = DatabaseManager::loadAdmins().size();
    int diningCount = DatabaseManager::loadDiningAuthorities().size();
    int totalUsers = studentCount + teacherCount + adminCount + diningCount;

    cout << "=== USER STATISTICS ===" << endl;
    cout << "Students: " << studentCount << endl;
    cout << "Teachers: " << teacherCount << endl;
    cout << "Admins: " << adminCount << endl;
    cout << "Dining Authorities: " << diningCount << endl;
    cout << "----------------------------------------" << endl;
    cout << "Total Users: " << totalUsers << endl;

    pauseForInput();
}

void AdminInterface::handleUserActivation() {
    displayHeader("USER ACTIVATION/DEACTIVATION");
    displayInfo("This feature will be implemented to manage user account status.");
    displayInfo("Future implementation: Enable/disable user accounts as needed.");
    pauseForInput();
}

void AdminInterface::handleSystemSettings() {
    displayHeader("SYSTEM SETTINGS");
    displayInfo("System settings features:");
    cout << "-> Configure meal time windows" << endl;
    cout << "-> Set system-wide policies" << endl;
    cout << "-> Manage hall configurations" << endl;
    cout << "-> Update system parameters" << endl;
    pauseForInput();
}

void AdminInterface::handleReports() {
    displayHeader("SYSTEM REPORTS");
    displayInfo("Comprehensive system reporting:");
    cout << "-> User activity reports" << endl;
    cout << "-> Meal token usage statistics" << endl;
    cout << "-> Revenue analytics across all halls" << endl;
    cout << "-> System performance metrics" << endl;
    pauseForInput();
}

void AdminInterface::handleNoticeManagement() {
    clearScreen();
    displayHeader("NOTICE MANAGEMENT");

    // Load existing notices from database instead of using static local vector
    vector<Notice> notices = DatabaseManager::loadNotices();

    while (true) {
        cout << "1. Create Notice" << endl;
        cout << "2. Update Notice" << endl;
        cout << "3. View All Notices" << endl;
        cout << "4. Back to Admin Menu" << endl;
        cout << "Enter choice from 1 to 4: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string title, message;
            cout << "Enter notice title: ";
            getline(cin, title);
            cout << "Enter message: ";
            getline(cin, message);

            Notice n(title, message, Date::getCurrentDate());

            // Save to database
            if (DatabaseManager::addNotice(n)) {
                displaySuccess("You've created the notice successfully!!!");
                // Reload notices from database to keep in sync
                notices = DatabaseManager::loadNotices();
            } else {
                displayError("Failed to save notice to database!");
            }

        } else if (choice == 2) {
            if (notices.empty()) {
                displayInfo("Nothing to update.");
            } else {
                for (int i = 0; i < notices.size(); i++) {
                    cout << i << ". " << notices[i].getTitle() << endl;
                }
                cout << "Enter notice index to update: ";
                int indx;
                cin >> indx;
                cin.ignore();
                if (indx < 0 || indx >= (int)notices.size()) {
                    displayError("Invalid index!");
                } else {
                    string nTitle, nMessage;
                    cout << "Enter new title: ";
                    getline(cin, nTitle);
                    cout << "Enter new message: ";
                    getline(cin, nMessage);
                    notices[indx].setTitle(nTitle);
                    notices[indx].setMessage(nMessage);

                    // Update in database
                    if (DatabaseManager::updateNotice(indx, notices[indx])) {
                        displaySuccess("Notice updated successfully!");
                    } else {
                        displayError("Failed to update notice in database!");
                    }
                }
            }

        } else if (choice == 3) {
            // Reload from database to show latest
            notices = DatabaseManager::loadNotices();

            if (notices.empty()) {
                displayInfo("Nothing to show.");
            } else {
                for (const auto& n : notices) {
                    cout << "\nTitle: " << n.getTitle() << endl;
                    cout << "Date: " << n.getDate().toString() << endl;
                    cout << "Message: " << n.getMessage() << endl;
                    displaySeparator('-', 50);
                }
            }

        } else if (choice == 4) {
            break;
        } else {
            displayError("The option is not valid!");
        }

        pauseForInput();
        clearScreen();
        displayHeader("NOTICE MANAGEMENT");
    }
}

void AdminInterface::handleDatabaseManagement() {
    displayHeader("DATABASE MANAGEMENT");
    displayInfo("Database management tools:");
    cout << "-> Backup and restore operations" << endl;
    cout << "-> Data cleanup and maintenance" << endl;
    cout << "-> System integrity checks" << endl;
    cout << "-> Performance optimization" << endl;
    pauseForInput();
}

void AdminInterface::handleProfile() {
    displayHeader("ADMIN PROFILE");
    currentAdmin->display();
    pauseForInput();
}

void AdminInterface::handleLogout() {
    if (confirmAction("Are you sure you want to logout?")) {
        displayInfo("Logged out successfully!");
        isRunning = false;
    }
}

void AdminInterface::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}
