//
// Created by Md. Asif Khan on 11/8/25.
// Dining Authority Interface Implementation
//

#include "DiningAuthorityInterface.h"
#include "../Authentication/auth.h"
#include <iostream>
#include <limits>

using namespace std;

DiningAuthorityInterface::DiningAuthorityInterface(DiningAuthority* authority, Auth* auth)
    : currentAuthority(authority), authSystem(auth), isRunning(true) {
    tokenManager = new TokenManager();
}

void DiningAuthorityInterface::displayAuthorityDashboard() {
    clearScreen();
    displayHeader("DINING AUTHORITY DASHBOARD");
    cout << "Welcome, " << currentAuthority->getName() << "!" << endl;
    cout << "Managing Hall: " << currentAuthority->getHallName() << endl;
    cout << "Email: " << currentAuthority->getEmail() << endl;
    displaySeparator('-', 60);
}

void DiningAuthorityInterface::displayMenu() {
    displayAuthorityDashboard();

    cout << "\nDINING AUTHORITY MENU:" << endl;
    cout << "1. Meal Management" << endl;
    cout << "2. Token Management" << endl;
    cout << "3. Review Management" << endl;
    cout << "4. Reports & Analytics" << endl;
    cout << "5. Notice Management" << endl;
    cout << "6. Profile Settings" << endl;
    cout << "7. Logout" << endl;
    displaySeparator('-', 40);
}

int DiningAuthorityInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (1-7): ";

    while (!(cin >> choice) || choice < 1 || choice > 7) {
        displayError("Invalid input! Please enter a number between 1-7.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (1-7): ";
    }

    cin.ignore();
    return choice;
}

void DiningAuthorityInterface::processChoice(int choice) {
    switch (choice) {
        case 1:
            handleMealManagement();
            break;
        case 2:
            handleTokenManagement();
            break;
        case 3:
            handleReviewManagement();
            break;
        case 4:
            handleReports();
            break;
        case 5:
            handleNoticeManagement();
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

void DiningAuthorityInterface::handleMealManagement() {
    while (true) {
        clearScreen();
        displayHeader("MEAL MANAGEMENT");

        cout << "\nMeal Operations:" << endl;
        cout << "1. Create New Meal" << endl;
        cout << "2. Update Existing Meal" << endl;
        cout << "3. View All Meals" << endl;
        cout << "4. View Today's Meals" << endl;
        cout << "5. View Upcoming Meals" << endl;
        cout << "6. Back to Main Menu" << endl;

        cout << "\nEnter choice (1-6): ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                handleCreateMeal();
                break;
            case 2:
                handleUpdateMeal();
                break;
            case 3:
                currentAuthority->viewAllMeals();
                pauseForInput();
                break;
            case 4:
                currentAuthority->viewTodaysMeals();
                pauseForInput();
                break;
            case 5:
                currentAuthority->viewUpcomingMeals();
                pauseForInput();
                break;
            case 6:
                return;
            default:
                displayError("Invalid choice!");
                pauseForInput();
        }
    }
}

void DiningAuthorityInterface::handleCreateMeal() {
    displayHeader("CREATE NEW MEAL");

    cout << "Enter meal name: ";
    string name;
    getline(cin, name);

    cout << "Enter meal description: ";
    string description;
    getline(cin, description);

    cout << "\nSelect meal type:" << endl;
    cout << "1. Breakfast" << endl;
    cout << "2. Lunch" << endl;
    cout << "3. Dinner" << endl;
    cout << "Enter choice (1-3): ";

    int typeChoice;
    cin >> typeChoice;
    cin.ignore();

    MealType type;
    switch (typeChoice) {
        case 1: type = MealType::BREAKFAST; break;
        case 2: type = MealType::LUNCH; break;
        case 3: type = MealType::DINNER; break;
        default:
            displayError("Invalid meal type!");
            pauseForInput();
            return;
    }

    cout << "Enter price: $";
    double price;
    cin >> price;
    cin.ignore();

    cout << "Enter quantity: ";
    int quantity;
    cin >> quantity;
    cin.ignore();

    cout << "Enter date (DD-MM-YYYY): ";
    string date;
    getline(cin, date);

    cout << "Enter time (HH:MM): ";
    string time;
    getline(cin, time);

    currentAuthority->createMeal(name, description, type, price, quantity, date, time);
    pauseForInput();
}

void DiningAuthorityInterface::handleUpdateMeal() {
    displayHeader("UPDATE MEAL");

    cout << "Enter meal name to update: ";
    string mealName;
    getline(cin, mealName);

    cout << "\nWhat would you like to update?" << endl;
    cout << "1. Availability Status" << endl;
    cout << "2. Quantity" << endl;
    cout << "3. Price" << endl;
    cout << "Enter choice (1-3): ";

    int updateChoice;
    cin >> updateChoice;
    cin.ignore();

    switch (updateChoice) {
        case 1: {
            cout << "Set availability (1-Available, 0-Unavailable): ";
            bool available;
            cin >> available;
            currentAuthority->updateMealAvailability(mealName, available);
            break;
        }
        case 2: {
            cout << "Enter new quantity: ";
            int quantity;
            cin >> quantity;
            currentAuthority->updateMealQuantity(mealName, quantity);
            break;
        }
        case 3: {
            cout << "Enter new price: $";
            double price;
            cin >> price;
            currentAuthority->updateMealPrice(mealName, price);
            break;
        }
        default:
            displayError("Invalid choice!");
    }

    pauseForInput();
}

void DiningAuthorityInterface::handleTokenManagement() {
    displayHeader("TOKEN MANAGEMENT");
    currentAuthority->viewActiveTokens();
    pauseForInput();
}

void DiningAuthorityInterface::handleReviewManagement() {
    while (true) {
        clearScreen();
        displayHeader("REVIEW MANAGEMENT");

        cout << "\nReview Options:" << endl;
        cout << "1. View All Hall Reviews" << endl;
        cout << "2. View Reviews by Meal" << endl;
        cout << "3. Review Analytics" << endl;
        cout << "4. Back to Main Menu" << endl;

        cout << "\nEnter choice (1-4): ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                currentAuthority->viewFoodReviews();
                pauseForInput();
                break;
            case 2: {
                cout << "Enter meal name: ";
                string mealName;
                getline(cin, mealName);
                currentAuthority->viewReviewsByMeal(mealName);
                pauseForInput();
                break;
            }
            case 3:
                displayInfo("Review analytics will be implemented with detailed statistics.");
                pauseForInput();
                break;
            case 4:
                return;
            default:
                displayError("Invalid choice!");
                pauseForInput();
        }
    }
}

void DiningAuthorityInterface::handleReports() {
    while (true) {
        clearScreen();
        displayHeader("REPORTS & ANALYTICS");

        cout << "\nReport Options:" << endl;
        cout << "1. Daily Report" << endl;
        cout << "2. Weekly Summary" << endl;
        cout << "3. Monthly Analytics" << endl;
        cout << "4. Revenue Report" << endl;
        cout << "5. Back to Main Menu" << endl;

        cout << "\nEnter choice (1-5): ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                cout << "Enter date for report (DD-MM-YYYY) or press Enter for today: ";
                string dateStr;
                getline(cin, dateStr);

                Date reportDate = dateStr.empty() ? Date() : Date(dateStr);
                currentAuthority->generateDailyReport(reportDate);
                pauseForInput();
                break;
            }
            case 2:
            case 3:
            case 4:
                displayInfo("Advanced reporting features will be implemented.");
                pauseForInput();
                break;
            case 5:
                return;
            default:
                displayError("Invalid choice!");
                pauseForInput();
        }
    }
}

void DiningAuthorityInterface::handleNoticeManagement() {
    while (true) {
        clearScreen();
        displayHeader("NOTICE MANAGEMENT");

        cout << "\nNotice Options:" << endl;
        cout << "1. Post New Notice" << endl;
        cout << "2. View All Notices" << endl;
        cout << "3. Back to Main Menu" << endl;

        cout << "\nEnter choice (1-3): ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                cout << "Enter notice title: ";
                string title;
                getline(cin, title);

                cout << "Enter notice content: ";
                string content;
                getline(cin, content);

                currentAuthority->postNotice(title, content);
                pauseForInput();
                break;
            }
            case 2:
                currentAuthority->viewNotices();
                pauseForInput();
                break;
            case 3:
                return;
            default:
                displayError("Invalid choice!");
                pauseForInput();
        }
    }
}

void DiningAuthorityInterface::handleProfile() {
    displayHeader("PROFILE SETTINGS");
    currentAuthority->display();
    pauseForInput();
}

void DiningAuthorityInterface::handleLogout() {
    if (confirmAction("Are you sure you want to logout?")) {
        displayInfo("Logged out successfully!");
        isRunning = false;
    }
}

void DiningAuthorityInterface::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}
