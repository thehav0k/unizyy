//
// Created by Md. Asif Khan on 15/8/25.
//

#ifndef UNIZYY_NOTICE_H
#define UNIZYY_NOTICE_H
#include "date.h"
#include "department.h"
#include "admintype.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
using namespace std;

// Comprehensive Notice Type Map
enum class NoticeType {
    ACADEMIC = 1,
    TRANSPORT = 2,
    DINING = 3,
    PUBLIC_RELATIONS = 4,
    DEPARTMENT = 5,
    SYSTEM_ADMIN = 6,
    EXAM = 7,
    ADMISSION = 8,
    RESULT = 9,
    SCHOLARSHIP = 10,
    EVENT = 11,
    MAINTENANCE = 12,
    EMERGENCY = 13,
    GENERAL = 14
};

// Notice Type to String mapping
class NoticeTypeHelper {
public:
    static const unordered_map<NoticeType, string> noticeTypeNames;
    static const unordered_map<string, NoticeType> stringToNoticeType;

    static string toString(NoticeType type);
    static NoticeType fromString(const string& typeStr);
    static vector<string> getAllNoticeTypes();
    static bool isValidNoticeType(const string& typeStr);
};

// class Notice {
// private:
//     static size_t nextNoticeID;
// protected:
//     size_t noticeID;
//     Date createdDate;
//     char title[150];
//     char description[500];
//     NoticeType noticeType;
//     department targetDepartment;

// public:
//     // Constructors
//     Notice();
//     Notice(const string& title, const string& description, NoticeType type);

//     // Getters
//     size_t getNoticeID() const;
//     Date getCreatedDate() const;
//     string getTitle() const;
//     string getDescription() const;
//     NoticeType getNoticeType() const;
//     string getNoticeTypeString() const;
//     department getTargetDepartment() const;

//     // Setters
//     void setTitle(const string& title);
//     void setDescription(const string& description);
//     void setNoticeType(NoticeType type);
//     void setTargetDepartment(department dept);

//     // Display methods
//     void displayNotice() const;
//     void displayDetailedNotice() const;
//     string getFormattedNotice() const;

//     // Static utility methods
//     static vector<Notice> filterNoticesByType(const vector<Notice>& notices, NoticeType type);
//     static void sortNoticesByDate(vector<Notice>& notices);

//     // File operations (to be used with DatabaseManager)
//     bool saveToFile(const string& filename) const;
//     static vector<Notice> loadFromFile(const string& filename);

//     // Comparison operators for sorting
//     bool operator<(const Notice& other) const;
//     bool operator>(const Notice& other) const;
// };

#endif //UNIZYY_NOTICE_H

