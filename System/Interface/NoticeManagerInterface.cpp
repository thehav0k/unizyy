#include "NoticeManagerInterface.h"
using namespace std;

NoticeInterface::NoticeInterface(Auth* auth, vector<Notice>& noticeBoard)
    : authSystem(auth), notices(noticeBoard), isRunning(true){}

void NoticeInterface::displayNoticeDashboard() {
    clearScreen();
    displayHeader("NOTICE BOARD");
    cout << "Role: " << (authSystem->getRole() == Role::ADMIN ? "Admin" : "Student") << "\n\n";

    if (authSystem->getRole() == Role::ADMIN) {
        cout << "Role: ADMIN - You can create, update, and deactivate notices.\n";
    } else {
        cout << "Role: STUDENT - You can only view active notices.\n";
    }
    displaySeparator('-', 60);
}

void NoticeInterface::displayMenu(bool isAdmin) {
    displayNoticeDashboard();

    cout << "NOTICE MENU OPTIONS:\n";
    if (isAdmin) {
        cout << "1. Create Notice\n";
        cout << "2. Update Notice\n";
        cout << "3. Deactivate Notice\n";
        cout << "4. View Notices\n";
        cout << "5. Logout\n";
        displaySeparator('-', 40);
    } else {
        cout << "1. View Notices\n";
        cout << "2. Logout\n";
        displaySeparator('-', 40);
    }
}

int NoticeInterface::getChoice(int maxChoice) {
    int choice;
    cout << "Please enter your choice (1-" << maxChoice << "): ";

    while (!(cin >> choice) || choice < 1 || choice > maxChoice) {
        displayError("Invalid input! Please enter a number between 1 and " + to_string(maxChoice) + ".");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter your choice (1-" << maxChoice << "): ";
    }

    cin.ignore();
    return choice;
}

void NoticeInterface::processChoice(int choice, bool isAdmin) {
    if (isAdmin) {
        switch (choice) {
            case 1: handleCreateNotice(); break;
            case 2: handleUpdateNotice(); break;
            case 3: handleViewNotices(); break;
            case 4: handleLogout(); break;
            default: displayError("Invalid choice!"); pauseForInput();
        }
    } else {
        switch (choice) {
            case 1: handleViewNotices(); break;
            case 2: handleLogout(); break;
            default: displayError("Invalid choice!"); pauseForInput();
        }
    }
}

// === Handlers ===

void NoticeInterface::handleCreateNotice() {
    displayHeader("CREATE NOTICE");
    string content;
    cout << "Enter notice content: ";
    getline(cin, content);

    if (!content.empty()) {
        Date currentDate; // Use current date
        notices.emplace_back("New Notice", content, currentDate);
        displaySuccess("Notice created successfully!");
    } else {
        displayError("Notice content cannot be empty.");
    }
    pauseForInput();
}

void NoticeInterface::handleUpdateNotice() {
    displayHeader("UPDATE NOTICE");
    if (notices.empty()) {
        displayError("No notices available to update.");
        pauseForInput();
        return;
    }

    for (size_t i = 0; i < notices.size(); i++) {
        cout << i << ". " << notices[i].getMessage()<< endl;
    }

    cout << "Enter index of notice to update: ";
    int idx; cin >> idx; cin.ignore();

    if (idx < 0 || idx >= (int)notices.size()) {
        displayError("Invalid index!");
    } else {
        cout << "Enter new content: ";
        string newContent;
        getline(cin, newContent);
        notices[idx].setMessage(newContent);
        displaySuccess("Notice updated successfully!");
    }
    pauseForInput();
}

void NoticeInterface::handleViewNotices() {
    displayHeader("VIEW NOTICES");
    if (notices.empty()) {
        displayInfo("No notices available.");
    } else {
        for (const auto& n : notices) {
                cout << "- " << n.getMessage() << endl;
        }
    }
    pauseForInput();
}

void NoticeInterface::handleLogout() {
    if (confirmAction("Are you sure you want to logout?")) {
        displayInfo("Logged out successfully!");
        isRunning = false;
    }
}

void NoticeInterface::run() {
    bool isAdmin = (authSystem->getRole() == Role::ADMIN);
    while (isRunning) {
        displayMenu(isAdmin);
        int maxChoice = isAdmin ? 5 : 2;
        int choice = getChoice(maxChoice);
        processChoice(choice, isAdmin);
    }
}
