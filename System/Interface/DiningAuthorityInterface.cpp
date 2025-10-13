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
    : currentAuthority(authority), authSystem(auth), tokenManager(nullptr), isRunning(true) {}

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
    cout << "2. View Student Reviews" << endl;
    cout << "3. Add Notice" << endl;
    cout << "4. View Profile" << endl;
    cout << "5. Logout" << endl;
    displaySeparator('-', 40);
}

int DiningAuthorityInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (1-5): ";
    while (!(cin >> choice) || choice < 1 || choice > 5) {
        displayError("Invalid input! Please enter a number between 1-5.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (1-5): ";
    }
    cin.ignore();
    return choice;
}

void DiningAuthorityInterface::processChoice(int choice) {
    switch (choice) {
        case 1: handleMealManagement(); break;
        case 2: handleViewReviews(); break;
        case 3: handleAddNotice(); break;
        case 4: handleProfile(); break;
        case 5: handleLogout(); break;
        default: displayError("Invalid choice!"); pauseForInput();
    }
}

void DiningAuthorityInterface::handleMealManagement() {
    while (true) {
        clearScreen();
        displayHeader("MEAL MANAGEMENT");
        cout << "1. Create New Meal" << endl;
        cout << "2. Update Meal" << endl;
        cout << "3. Delete Meal" << endl;
        cout << "4. View All Meals" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "Enter choice (1-5): ";

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
            case 3: {
                currentAuthority->viewAllMeals();
                cout << "\nEnter meal ID to delete: ";
                int idx;
                cin >> idx;
                cin.ignore();
                if (confirmAction("Are you sure you want to delete this meal?")) {
                    currentAuthority->deleteMeal(idx);
                }
                pauseForInput();
            } break;
            case 4:
                currentAuthority->viewAllMeals();
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

void DiningAuthorityInterface::handleCreateMeal() {
    displayHeader("CREATE NEW MEAL");

    string name, description, date, time;
    int typeChoice;
    double price;
    int quantity;

    cout << "Enter meal name: ";
    getline(cin, name);
    cout << "Enter meal description: ";
    getline(cin, description);
    cout << "Meal type (1=Breakfast, 2=Lunch, 3=Dinner): ";
    cin >> typeChoice;
    cin.ignore();

    MealType type = MealType::BREAKFAST;
    if (typeChoice == 2) type = MealType::LUNCH;
    else if (typeChoice == 3) type = MealType::DINNER;

    cout << "Enter price: ";
    cin >> price;
    cin.ignore();
    cout << "Enter quantity: ";
    cin >> quantity;
    cin.ignore();
    cout << "Enter date (DD-MM-YYYY): ";
    getline(cin, date);
    cout << "Enter time (HH:MM): ";
    getline(cin, time);

    currentAuthority->createMeal(name, description, type, price, quantity, date, time);
    pauseForInput();
}

void DiningAuthorityInterface::handleUpdateMeal() {
    displayHeader("UPDATE MEAL");

    currentAuthority->viewAllMeals();

    cout << "\nEnter meal ID to update: ";
    int idx;
    cin >> idx;
    cin.ignore();

    string newName, newDesc;
    double newPrice;
    int newQty;
    int typeChoice;

    cout << "New name (leave empty to keep current): ";
    getline(cin, newName);
    cout << "New description (leave empty to keep current): ";
    getline(cin, newDesc);
    cout << "New meal type (1=Breakfast, 2=Lunch, 3=Dinner): ";
    cin >> typeChoice;
    cin.ignore();

    MealType newType = MealType::BREAKFAST;
    if (typeChoice == 2) newType = MealType::LUNCH;
    else if (typeChoice == 3) newType = MealType::DINNER;

    cout << "New price (-1 to keep current): ";
    cin >> newPrice;
    cin.ignore();
    cout << "New quantity (-1 to keep current): ";
    cin >> newQty;
    cin.ignore();

    currentAuthority->updateMeal(idx, newName, newDesc, newType, newPrice, newQty);
    pauseForInput();
}

void DiningAuthorityInterface::handleViewReviews() {
    clearScreen();
    displayHeader("STUDENT MEAL REVIEWS");
    currentAuthority->viewFoodReviews();
    pauseForInput();
}

void DiningAuthorityInterface::handleAddNotice() {
    clearScreen();
    displayHeader("ADD NOTICE");

    string title, message;
    cout << "Enter notice title: ";
    getline(cin, title);
    cout << "Enter notice message: ";
    getline(cin, message);

    currentAuthority->addNotice(title, message);
    pauseForInput();
}

void DiningAuthorityInterface::handleProfile() {
    clearScreen();
    displayHeader("PROFILE");
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
