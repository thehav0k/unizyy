//
// Created by Md. Asif Khan on 15/8/25.
//

#include "Notice.h"
#include "../Database/DatabaseManager.h"
#include "../Utils/StringHelper.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// Initialize static member
size_t Notice::nextNoticeID = 20250815;

// NoticeTypeHelper implementation
const unordered_map<NoticeType, string> NoticeTypeHelper::noticeTypeNames = {
    {NoticeType::ACADEMIC, "Academic"},
    {NoticeType::TRANSPORT, "Transport"},
    {NoticeType::DINING, "Dining"},
    {NoticeType::PUBLIC_RELATIONS, "Public Relations"},
    {NoticeType::DEPARTMENT, "Department"},
    {NoticeType::SYSTEM_ADMIN, "System Admin"},
    {NoticeType::EXAM, "Exam"},
    {NoticeType::ADMISSION, "Admission"},
    {NoticeType::RESULT, "Result"},
    {NoticeType::SCHOLARSHIP, "Scholarship"},
    {NoticeType::EVENT, "Event"},
    {NoticeType::MAINTENANCE, "Maintenance"},
    {NoticeType::EMERGENCY, "Emergency"},
    {NoticeType::GENERAL, "General"}
};

const unordered_map<string, NoticeType> NoticeTypeHelper::stringToNoticeType = {
    {"Academic", NoticeType::ACADEMIC},
    {"Transport", NoticeType::TRANSPORT},
    {"Dining", NoticeType::DINING},
    {"Public Relations", NoticeType::PUBLIC_RELATIONS},
    {"Department", NoticeType::DEPARTMENT},
    {"System Admin", NoticeType::SYSTEM_ADMIN},
    {"Exam", NoticeType::EXAM},
    {"Admission", NoticeType::ADMISSION},
    {"Result", NoticeType::RESULT},
    {"Scholarship", NoticeType::SCHOLARSHIP},
    {"Event", NoticeType::EVENT},
    {"Maintenance", NoticeType::MAINTENANCE},
    {"Emergency", NoticeType::EMERGENCY},
    {"General", NoticeType::GENERAL}
};

string NoticeTypeHelper::toString(NoticeType type) {
    auto it = noticeTypeNames.find(type);
    return (it != noticeTypeNames.end()) ? it->second : "Unknown";
}

NoticeType NoticeTypeHelper::fromString(const string& typeStr) {
    auto it = stringToNoticeType.find(typeStr);
    return (it != stringToNoticeType.end()) ? it->second : NoticeType::GENERAL;
}

vector<string> NoticeTypeHelper::getAllNoticeTypes() {
    vector<string> types;
    for (const auto& pair : noticeTypeNames) {
        types.push_back(pair.second);
    }
    return types;
}

bool NoticeTypeHelper::isValidNoticeType(const string& typeStr) {
    return stringToNoticeType.find(typeStr) != stringToNoticeType.end();
}

// Constructors
Notice::Notice() : noticeID(nextNoticeID++), createdDate(Date::getCurrentDate()),
                   noticeType(NoticeType::GENERAL),
                   targetDepartment(department::Department_of_Computer_Science_and_Engineering),
                   isActive(true) {
    title[0] = '\0';
    description[0] = '\0';
    authorName[0] = '\0';
    authorEmail[0] = '\0';
    targetAudience[0] = '\0';
}

Notice::Notice(const string& title, const string& description, NoticeType type,
               const string& authorName, const string& authorEmail)
    : noticeID(nextNoticeID++), createdDate(Date::getCurrentDate()),
      noticeType(type),
      targetDepartment(department::Department_of_Computer_Science_and_Engineering),
      isActive(true) {
    StringHelper::stringToCharArray(title, this->title);
    StringHelper::stringToCharArray(description, this->description);
    StringHelper::stringToCharArray(authorName, this->authorName);
    StringHelper::stringToCharArray(authorEmail, this->authorEmail);
    StringHelper::stringToCharArray("All", this->targetAudience);
}

// Getters
size_t Notice::getNoticeID() const { return noticeID; }
Date Notice::getCreatedDate() const { return createdDate; }
string Notice::getTitle() const { return StringHelper::charArrayToString(title); }
string Notice::getDescription() const { return StringHelper::charArrayToString(description); }
string Notice::getAuthorName() const { return StringHelper::charArrayToString(authorName); }
string Notice::getAuthorEmail() const { return StringHelper::charArrayToString(authorEmail); }
NoticeType Notice::getNoticeType() const { return noticeType; }
string Notice::getNoticeTypeString() const { return NoticeTypeHelper::toString(noticeType); }
department Notice::getTargetDepartment() const { return targetDepartment; }
string Notice::getTargetAudience() const { return StringHelper::charArrayToString(targetAudience); }
bool Notice::getIsActive() const { return isActive; }

// Setters
void Notice::setTitle(const string& title) {
    StringHelper::stringToCharArray(title, this->title);
}

void Notice::setDescription(const string& description) {
    StringHelper::stringToCharArray(description, this->description);
}

void Notice::setTargetDepartment(department dept) {
    this->targetDepartment = dept;
}

void Notice::setTargetAudience(const string& audience) {
    StringHelper::stringToCharArray(audience, this->targetAudience);
}

void Notice::setActive(bool active) {
    this->isActive = active;
}

// Display methods
void Notice::displayNotice() const {
    cout << "Notice ID: " << noticeID << endl;
    cout << "Title: " << getTitle() << endl;
    cout << "Type: " << getNoticeTypeString() << endl;
    cout << "Author: " << getAuthorName() << endl;
}

void Notice::displayDetailedNotice() const {
    displayNotice();
    cout << "Description: " << getDescription() << endl;
    cout << "Created: " << createdDate.toString() << endl;
    cout << "Target: " << getTargetAudience() << endl;
    cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
}

string Notice::getFormattedNotice() const {
    return getTitle() + " - " + getAuthorName();
}

// Static utility methods
vector<Notice> Notice::filterNoticesByType(const vector<Notice>& notices, NoticeType type) {
    vector<Notice> filtered;
    for (const auto& notice : notices) {
        if (notice.getNoticeType() == type) {
            filtered.push_back(notice);
        }
    }
    return filtered;
}

vector<Notice> Notice::getActiveNotices(const vector<Notice>& notices) {
    vector<Notice> active;
    for (const auto& notice : notices) {
        if (notice.getIsActive()) {
            active.push_back(notice);
        }
    }
    return active;
}

void Notice::sortNoticesByDate(vector<Notice>& notices) {
    sort(notices.begin(), notices.end(), [](const Notice& a, const Notice& b) {
        return a.getCreatedDate() > b.getCreatedDate();
    });
}

// File operations using DatabaseManager
bool Notice::saveToFile(const string& filename) const {
    // Load existing notices
    vector<Notice> notices = loadFromFile(filename);

    // Add this notice to the collection
    notices.push_back(*this);

    // Save all notices using DatabaseManager
    DatabaseManager::saveObjects(notices, filename);
    return true;
}

vector<Notice> Notice::loadFromFile(const string& filename) {
    vector<Notice> notices;
    DatabaseManager::loadObjects(notices, filename);
    return notices;
}

// Comparison operators for sorting
bool Notice::operator<(const Notice& other) const {
    return this->createdDate < other.createdDate;
}

bool Notice::operator>(const Notice& other) const {
    return this->createdDate > other.createdDate;
}
