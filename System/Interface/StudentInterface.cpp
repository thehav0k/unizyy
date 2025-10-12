//
// Created by Md. Asif Khan on 11/8/25.
// Student Interface Implementation
//

#include "StudentInterface.h"
#include "../Authentication/auth.h"
#include "../../Core/Models/date.h"
#include "../../Core/Models/Notice.h"
#include "../../Core/Database/DatabaseManager.h"
#include "../../Core/Utils/StringHelper.h"
#include <iostream>
#include <limits>
#include <iomanip>

using namespace std;

StudentInterface::StudentInterface(Student* student, Auth* auth)
    : currentStudent(student), authSystem(auth), isRunning(true) {
    tokenManager = new TokenManager();
}

void StudentInterface::displayStudentDashboard() {
    clearScreen();
    displayHeader("STUDENT DASHBOARD");
    cout << "Welcome, " << currentStudent->getName() << "!" << endl;
    cout << "Student ID: " << currentStudent->getStudentID() << endl;
    cout << "Hall: " << hallToString(currentStudent->getHall()) << endl;
    displaySeparator('-', 60);
}

void StudentInterface::displayMenu() {
    displayStudentDashboard();
    cout << "\nSTUDENT MENU OPTIONS:" << endl;
    cout << "0. Simulate Date" << endl;
    cout << "1. Meal Token Operations" << endl;
    cout << "2. View Profile" << endl;
    cout << "3. Change Password" << endl;
    cout << "4. View Notices" << endl;
    cout << "5. Logout" << endl;
    displaySeparator('-', 40);
}

int StudentInterface::getChoice() {
    int choice;
    cout << "Please enter your choice (0-5): ";

    while (!(cin >> choice) || choice < 0 || choice > 6) {
        displayError("Invalid input! Please enter a number between 0-4.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (0-4): ";
    }

    cin.ignore();
    return choice;
}

void StudentInterface::processChoice(int choice) {
    switch (choice) {
        case 0: {
            displayHeader("SIMULATE DATE & TIME");
            cout << "1. Set specific date (DD-MM-YYYY)" << endl;
            cout << "2. Advance N days" << endl;
            cout << "3. Advance N months" << endl;
            cout << "4. Advance N hours" << endl;
            cout << "5. Set date + hour" << endl;
            cout << "6. Reset simulation" << endl;
            cout << "Enter choice (1-6): ";
            string in; getline(cin, in); int simChoice = in.empty()?0:stoi(in);
            switch(simChoice){
                case 1: { cout << "Enter date (DD-MM-YYYY): "; string d; getline(cin,d); if(d.empty()) displayError("Empty date"); else { Date dt(d); if(!dt.isValid()) displayError("Invalid date"); else { Date::setSimulatedDate(dt); displaySuccess("Simulation date set to "+dt.toString()); } } } break;
                case 2: { cout << "Days to advance: "; size_t n; cin>>n; cin.ignore(); Date dt=Date::SimulateDate(n); displaySuccess("Advanced to "+dt.toString()); } break;
                case 3: { cout << "Months to advance: "; size_t n; cin>>n; cin.ignore(); Date dt=Date::SimulateMonths(n); displaySuccess("Advanced to "+dt.toString()); } break;
                case 4: { cout << "Hours to advance: "; size_t n; cin>>n; cin.ignore(); Date::SimulateHours(n); Date dt=Date::getCurrentDate(); int h=Date::getSimulatedHour(); displaySuccess("Time now "+dt.toString()+" "+to_string(h)+":00"); } break;
                case 5: { cout << "Enter date (DD-MM-YYYY): "; string d; getline(cin,d); cout << "Enter hour (0-23): "; int h; cin>>h; cin.ignore(); Date dt(d); if(!dt.isValid()) displayError("Invalid date"); else { Date::setSimulatedDateTime(dt,h); displaySuccess("Set to "+dt.toString()+" "+to_string(Date::getSimulatedHour())+":00"); } } break;
                case 6: { Date::resetSimulation(); displaySuccess("Simulation reset."); } break;
                default: displayInfo("Cancelled.");
            }
            pauseForInput();
        } break;
        case 1:
            handleMealTokenOperations();
            break;
        case 2:
            handleViewProfile();
            break;
        case 3:
            handleChangePassword();
            break;
        case 4:
            handleViewNotices();
        case 5:
            handleLogout();
            break;
        default:
            displayError("Invalid choice!");
            pauseForInput();
    }
}

void StudentInterface::handleMealTokenOperations() {
    while (true) {
        clearScreen();
        displayHeader("MEAL TOKEN OPERATIONS");

        cout << "\nToken Menu:" << endl;
        cout << "1. Buy Meal Token" << endl;
        cout << "2. Use Meal Token" << endl;
        cout << "3. View My Tokens" << endl;
        cout << "4. Review Meal" << endl;
        cout << "5. Back to Main Menu" << endl;

        cout << "\nEnter choice (1-5): ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                handleBuyToken();
                break;
            case 2:
                handleUseToken();
                break;
            case 3:
                handleViewMyTokens();
                break;
            case 4:
                handleReviewMeal();
                break;
            case 5:
                return;
            default:
                displayError("Invalid choice!");
                pauseForInput();
        }
    }
}

void StudentInterface::handleBuyToken() {
    displayHeader("BUY MEAL TOKEN");
    displayAvailableHalls();

    cout << "\nEnter hall choice: ";
    int hallChoice;
    cin >> hallChoice;
    cin.ignore();

    vector<string> halls = MealUtils::getAvailableHalls();
    if (hallChoice < 1 || hallChoice > (int)halls.size()) {
        displayError("Invalid hall selection!");
        pauseForInput();
        return;
    }

    string selectedHall = halls[hallChoice - 1];

    displayMealTypes();
    cout << "\nEnter meal type (1-3): ";
    int mealTypeChoice;
    cin >> mealTypeChoice;
    cin.ignore();

    MealType selectedType;
    switch (mealTypeChoice) {
        case 1: selectedType = MealType::BREAKFAST; break;
        case 2: selectedType = MealType::LUNCH; break;
        case 3: selectedType = MealType::DINNER; break;
        default:
            displayError("Invalid meal type!");
            pauseForInput();
            return;
    }

    displayMealDetailsForPurchase(selectedHall, selectedType);

    cout << "\nDo you want to proceed with purchasing this meal token? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();

    if (confirm != 'y' && confirm != 'Y') {
        displayInfo("Token purchase cancelled.");
        pauseForInput();
        return;
    }

    Date tomorrow = Date::getTomorrowDate();
    if (!tokenManager->canBuyToken(currentStudent->getEmail(), selectedType, tomorrow)) {
        displayError("You already have a token for this meal type tomorrow!");
        pauseForInput();
        return;
    }

    string mealName = "Sample " + Meal::mealTypeToString(selectedType);
    Meal sampleMeal(mealName, "Delicious meal from " + selectedHall,
                   selectedType, 85.50, 50, tomorrow.toString(), "12:00", selectedHall);

    // Buy token
    string tokenNumber = tokenManager->buyToken(currentStudent->getEmail(), selectedHall, selectedType, sampleMeal);

    if (!tokenNumber.empty()) {
        displaySuccess("Token purchased successfully!");
        cout << "\nYour Token Details:" << endl;
        cout << "Token Number: " << tokenNumber << endl;
        cout << "Hall: " << selectedHall << endl;
        cout << "Meal Type: " << Meal::mealTypeToString(selectedType) << endl;
        cout << "Valid For: " << tomorrow.toString() << endl;
        cout << "Amount Paid: ৳85.50 BDT" << endl;

        displayInfo("Token details saved in 'Meal Tokens' folder.");
    } else {
        displayError("Failed to purchase token!");
    }

    pauseForInput();
}

void StudentInterface::handleUseToken() {
    displayHeader("USE MEAL TOKEN");

    cout << "Enter your token number: ";
    string tokenNumber;
    getline(cin, tokenNumber);

    if (tokenManager->useToken(tokenNumber, currentStudent->getEmail())) {
        displaySuccess("Token used successfully! Enjoy your meal!");
        displayInfo("You can now review this meal after eating.");
    } else {
        displayError("Invalid token or token cannot be used now!");
        cout << "\nPlease ensure:" << endl;
        cout << "• Token is valid and active" << endl;
        cout << "• It's the correct date for the meal" << endl;
        cout << "• You're within the meal time window" << endl;
        cout << "• Breakfast: 7:00 AM - 10:00 AM" << endl;
        cout << "• Lunch: 12:00 PM - 2:30 PM" << endl;
        cout << "• Dinner: 7:00 PM - 10:30 PM" << endl;
    }

    pauseForInput();
}

void StudentInterface::handleViewMyTokens() {
    displayHeader("MY MEAL TOKENS");
    tokenManager->displayStudentTokens(currentStudent->getEmail());
    pauseForInput();
}

void StudentInterface::handleReviewMeal() {
    displayHeader("REVIEW MEAL");

    cout << "Enter token number of consumed meal: ";
    string tokenNumber;
    getline(cin, tokenNumber);

    cout << "Rate your meal (1-5): ";
    int rating;
    cin >> rating;
    cin.ignore();

    if (rating < 1 || rating > 5) {
        displayError("Invalid rating! Must be between 1-5.");
        pauseForInput();
        return;
    }

    cout << "Enter your review comment: ";
    string comment;
    getline(cin, comment);

    MealRating mealRating = static_cast<MealRating>(rating);

    if (tokenManager->addReview(currentStudent->getEmail(), tokenNumber, mealRating, comment)) {
        displaySuccess("Review submitted successfully! Thank you for your feedback!");
    } else {
        displayError("Failed to submit review!");
        cout << "Please ensure the token was used and belongs to you." << endl;
    }

    pauseForInput();
}

void StudentInterface::displayAvailableHalls() {
    vector<string> halls = MealUtils::getAvailableHalls();
    cout << "\nAvailable Dining Halls:" << endl;
    for (size_t i = 0; i < halls.size(); ++i) {
        cout << (i + 1) << ". " << halls[i] << endl;
    }
}

void StudentInterface::displayMealTypes() {
    cout << "\nMeal Types:" << endl;
    cout << "1. Breakfast (7:00 AM - 10:00 AM)" << endl;
    cout << "2. Lunch (12:00 PM - 2:30 PM)" << endl;
    cout << "3. Dinner (7:00 PM - 10:30 PM)" << endl;
}

void StudentInterface::displayMealDetailsForPurchase(const string& hallName, MealType mealType) {
    displayHeader("MEAL DETAILS PREVIEW");

    Date tomorrow = Date::getTomorrowDate();

    // Try to load actual meal data from database
    vector<Meal> availableMeals = Meal::loadMealsByHall(hallName);
    vector<Meal> matchingMeals;

    // Filter meals by type and date
    for (const auto& meal : availableMeals) {
        if (meal.getMealType() == mealType && meal.getDate() == tomorrow.toString()) {
            matchingMeals.push_back(meal);
        }
    }

    if (!matchingMeals.empty()) {
        cout << "🍽️  TOMORROW'S " << Meal::mealTypeToString(mealType) << " AT " << hallName << endl;
        displaySeparator('=', 60);

        const Meal& meal = matchingMeals[0];
        meal.displayMeal();

    } else {
        cout << "🍽️  SAMPLE " << Meal::mealTypeToString(mealType) << " AT " << hallName << endl;
        displaySeparator('=', 60);

        cout << "📋 Meal Details:" << endl;
        cout << "   Meal Name: Sample " << Meal::mealTypeToString(mealType) << endl;
        cout << "   Description: Delicious meal from " << hallName << endl;

        // Display sample meal items based on meal type
        cout << "🥘 Meal Items:" << endl;
        switch (mealType) {
            case MealType::BREAKFAST:
                cout << "   • Paratha/Roti with Butter" << endl;
                cout << "   • Omelet/Boiled Egg" << endl;
                cout << "   • Mixed Vegetables" << endl;
                cout << "   • Tea/Coffee" << endl;
                cout << "   • Fruits (Seasonal)" << endl;
                break;
            case MealType::LUNCH:
                cout << "   • Steamed Rice" << endl;
                cout << "   • Fish/Chicken Curry" << endl;
                cout << "   • Dal (Lentil Soup)" << endl;
                cout << "   • Mixed Vegetables" << endl;
                cout << "   • Salad" << endl;
                break;
            case MealType::DINNER:
                cout << "   • Rice/Roti" << endl;
                cout << "   • Meat/Fish Curry" << endl;
                cout << "   • Dal" << endl;
                cout << "   • Vegetables" << endl;
                cout << "   • Sweet Dish" << endl;
                break;
        }

        cout << "💰 Price: ৳85.50 BDT" << endl;
        cout << "📅 Valid For: " << tomorrow.toString() << endl;
        cout << "🏛️  Hall: " << hallName << endl;
        cout << "✅ Availability: Available" << endl;
        cout << "📦 Quantity: 50+ portions available" << endl;

        displayInfo("Note: This is sample meal information. Actual meals may vary.");
    }

    displaySeparator('-', 60);
}

void StudentInterface::handleViewProfile() {
    displayHeader("STUDENT PROFILE");
    currentStudent->display();
    pauseForInput();
}

void StudentInterface::handleChangePassword() {
    displayHeader("CHANGE PASSWORD");

    cout << "Enter current password: ";
    string currentPassword;
    getline(cin, currentPassword);

    if (currentPassword != currentStudent->getPassword()) {
        displayError("Incorrect current password!");
        pauseForInput();
        return;
    }

    cout << "Enter new password: ";
    string newPassword;
    getline(cin, newPassword);

    cout << "Confirm new password: ";
    string confirmPassword;
    getline(cin, confirmPassword);

    if (newPassword != confirmPassword) {
        displayError("Passwords don't match!");
        pauseForInput();
        return;
    }

    if (!StringHelper::validatePassword(newPassword)) {
        displayError("New password doesn't meet requirements:");
        cout << "   • At least 6 characters long" << endl;
        cout << "   • Must contain at least one uppercase letter" << endl;
        cout << "   • Must contain at least one lowercase letter" << endl;
        cout << "   • Must contain at least one digit" << endl;
        pauseForInput();
        return;
    }

    currentStudent->setPassword(newPassword);

    if (DatabaseManager::updateStudent(currentStudent->getStudentID(), *currentStudent)) {
        displaySuccess("Password changed successfully and saved to database!");
    } else {
        displayError("Password changed in session but failed to save to database!");
    }

    pauseForInput();
}

//Notice

void StudentInterface::handleViewNotices() {
    displayHeader("VIEW NOTICES");
    vector<Notice> notices = DatabaseManager::loadNotices();

    if (notices.empty()) {
        displayInfo("No notices  available");
    } else {
        for (size_t i = 0; i < notices.size(); ++i) {
            cout << "\nNotice " << (i + 1) << ":" << endl;
            cout << "Title: " << notices[i].getTitle() << endl;
            cout << "Date: " << notices[i].getDate().toString() << endl;
            cout << "Message: " << notices[i].getMessage() << endl;
            displaySeparator('-', 50);
        }
    }

    pauseForInput();
}


void StudentInterface::handleLogout() {
    displayHeader("LOGOUT");
    if (confirmAction("Are you sure you want to logout?")) {
        displaySuccess("Logged out successfully!");
        isRunning = false;
    }
}

void StudentInterface::run() {
    while (isRunning) {
        displayMenu();
        int choice = getChoice();
        processChoice(choice);
    }
}
