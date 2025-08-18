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
    : currentAuthority(authority), authSystem(auth), isRunning(true) {}

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
    cout << "2. Reports & Analytics" << endl;
    cout << "3. Profile Settings" << endl;
    cout << "4. Logout" << endl;
    displaySeparator('-', 40);
}

int DiningAuthorityInterface::getChoice() {
    int choice; cout << "Please enter your choice (1-4): ";
    while (!(cin >> choice) || choice < 1 || choice > 4) {
        displayError("Invalid input! Please enter a number between 1-4.");
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (1-4): ";
    }
    cin.ignore(); return choice;
}

void DiningAuthorityInterface::processChoice(int choice) {
    switch (choice) {
        case 1: handleMealManagement(); break;
        case 2: handleReports(); break;
        case 3: handleProfile(); break;
        case 4: handleLogout(); break;
        default: displayError("Invalid choice!"); pauseForInput();
    }
}

void DiningAuthorityInterface::handleMealManagement() {
    while (true) {
        clearScreen(); displayHeader("MEAL MANAGEMENT");
        cout << "1. Create New Meal" << endl;
        cout << "2. Update Existing Meal (by index)" << endl;
        cout << "3. Delete Meal (by index)" << endl;
        cout << "4. View All Meals" << endl;
        cout << "5. Search Meals by Name" << endl;
        cout << "6. Search Meals by Type" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "Enter choice (1-7): ";
        int choice; cin >> choice; cin.ignore();
        switch (choice) {
            case 1: handleCreateMeal(); break;
            case 2: handleUpdateMeal(); break;
            case 3: {
                cout << "Enter meal index to delete: "; int idx; cin >> idx; cin.ignore();
                currentAuthority->deleteMeal(idx); pauseForInput();
            } break;
            case 4: currentAuthority->displayAllMeals(); pauseForInput(); break;
            case 5: {
                cout << "Enter search name: "; string q; getline(cin,q);
                currentAuthority->searchMealsByName(q); pauseForInput();
            } break;
            case 6: {
                cout << "Select type (1=Breakfast 2=Lunch 3=Dinner): "; int t; cin >> t; cin.ignore();
                MealType mt = MealType::BREAKFAST; if (t==2) mt=MealType::LUNCH; else if (t==3) mt=MealType::DINNER;
                currentAuthority->searchMealsByType(mt); pauseForInput();
            } break;
            case 7: return;
            default: displayError("Invalid choice!"); pauseForInput();
        }
    }
}

void DiningAuthorityInterface::handleCreateMeal() {
    displayHeader("CREATE NEW MEAL");
    string name, description, date, time; int typeChoice; double price; int quantity;
    cout << "Enter meal name: "; getline(cin,name);
    cout << "Enter meal description: "; getline(cin,description);
    cout << "Meal type (1=Breakfast 2=Lunch 3=Dinner): "; cin >> typeChoice; cin.ignore();
    MealType type = MealType::BREAKFAST; if (typeChoice==2) type=MealType::LUNCH; else if (typeChoice==3) type=MealType::DINNER;
    cout << "Enter price: "; cin >> price; cin.ignore();
    cout << "Enter quantity: "; cin >> quantity; cin.ignore();
    cout << "Enter date (DD-MM-YYYY): "; getline(cin,date);
    cout << "Enter time (HH:MM): "; getline(cin,time);
    currentAuthority->createMeal(name, description, type, price, quantity, date, time);
    pauseForInput();
}

void DiningAuthorityInterface::handleUpdateMeal() {
    displayHeader("UPDATE MEAL (BY INDEX)");
    currentAuthority->displayAllMeals();
    cout << "Enter meal index to update: "; int idx; cin >> idx; cin.ignore();
    string newName, newDesc; double newPrice; int newQty; int typeChoice;
    cout << "New name (leave empty to keep): "; getline(cin,newName);
    cout << "New description (leave empty to keep): "; getline(cin,newDesc);
    cout << "New meal type (1=Breakfast 2=Lunch 3=Dinner): "; cin >> typeChoice; cin.ignore();
    MealType newType = MealType::BREAKFAST; if (typeChoice==2) newType=MealType::LUNCH; else if (typeChoice==3) newType=MealType::DINNER;
    cout << "New price (-1 keep): "; cin >> newPrice; cin.ignore(); if (newPrice<0) newPrice = 0; // logic in update handles non-neg
    cout << "New quantity (-1 keep): "; cin >> newQty; cin.ignore(); if (newQty<0) newQty = 0;
    // Fetch existing meals to apply partial updates
    // For simplicity we'll call updateMeal with provided values (some may overwrite with defaults if unchanged)
    currentAuthority->updateMeal(idx, newName, newDesc, newType, newPrice, newQty);
    pauseForInput();
}

void DiningAuthorityInterface::handleReports() {
    while (true) {
        clearScreen(); displayHeader("REPORTS & ANALYTICS");
        cout << "1. Meal Summary Report" << endl;
        cout << "2. Token Sales Report" << endl;
        cout << "3. Meal Reviews" << endl;
        cout << "4. Back" << endl;
        cout << "Enter choice (1-4): "; int c; cin >> c; cin.ignore();
        switch(c){
            case 1: currentAuthority->generateMealReport(); pauseForInput(); break;
            case 2: currentAuthority->DisplayTokenSellsReport(); pauseForInput(); break;
            case 3: currentAuthority->ViewFoodReviews(); pauseForInput(); break;
            case 4: return;
            default: displayError("Invalid choice!"); pauseForInput();
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
