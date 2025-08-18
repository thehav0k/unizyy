//
// Created by Md. Asif Khan on 15/8/25.
//

#include "NoticeManager.h"
#include <algorithm>
#include <iostream>

using namespace std;

// Core notice operations
vector<Notice> NoticeManager::getAllNotices() const {
    return Notice::loadFromFile(noticeDbFile);
}

bool NoticeManager::saveNotice(const Notice& notice) const {
    return notice.saveToFile(noticeDbFile);
}

bool NoticeManager::updateNoticeById(size_t id, const Notice& updatedNotice, const string& authorEmail) const {
    vector<Notice> allNotices = getAllNotices();
    bool found = false;

    for (auto& notice : allNotices) {
        if (notice.getNoticeID() == id) {
            notice = updatedNotice;
            found = true;
            break;
        }
    }

    if (found) {
        DatabaseManager::saveObjects(allNotices, noticeDbFile);
        return true;
    }
    return false;
}

bool NoticeManager::removeNoticeById(size_t id, const string& authorEmail) const {
    vector<Notice> allNotices = getAllNotices();

    auto it = allNotices.begin();
    while (it != allNotices.end()) {
        if (it->getNoticeID() == id) {
            allNotices.erase(it);
            DatabaseManager::saveObjects(allNotices, noticeDbFile);
            return true;
        } else {
            it++;
        }
    }
    return false;
}

// Generic filtering methods
vector<Notice> NoticeManager::filterNotices(const function<bool(const Notice&)>& predicate) const {
    vector<Notice> allNotices = getAllNotices();
    vector<Notice> filteredNotices;

    for (const auto& notice : allNotices) {
        if (predicate(notice)) {
            filteredNotices.push_back(notice);
        }
    }
    return filteredNotices;
}

vector<Notice> NoticeManager::getNoticesByAuthor(const string& authorEmail) const {
    // Since we removed author tracking, return notices by department instead
    return filterNotices([](const Notice& notice) {
        return true; // Return all notices since we can't filter by author anymore
    });
}

vector<Notice> NoticeManager::getNoticesByType(NoticeType type) const {
    return filterNotices([type](const Notice& notice) {
        return notice.getNoticeType() == type;
    });
}

vector<Notice> NoticeManager::getActiveNotices() const {
    // Since we removed isActive field, return all notices
    return getAllNotices();
}

vector<Notice> NoticeManager::getNoticesByDateRange(const Date& startDate, const Date& endDate) const {
    return filterNotices([&startDate, &endDate](const Notice& notice) {
        Date noticeDate = notice.getCreatedDate();
        return noticeDate >= startDate && noticeDate <= endDate;
    });
}

// Generic search method
vector<Notice> NoticeManager::searchNotices(const string& keyword, bool searchTitle, bool searchContent) const {
    return filterNotices([&keyword, searchTitle, searchContent](const Notice& notice) {
        bool titleMatch = searchTitle && notice.getTitle().find(keyword) != string::npos;
        bool contentMatch = searchContent && notice.getDescription().find(keyword) != string::npos;
        return titleMatch || contentMatch;
    });
}

// Utility methods
Notice* NoticeManager::findNoticeByID(size_t noticeID) const {
    static vector<Notice> allNotices = getAllNotices();

    for (auto& notice : allNotices) {
        if (notice.getNoticeID() == noticeID) {
            return &notice;
        }
    }
    return nullptr;
}

bool NoticeManager::isNoticeOwner(size_t noticeID, const string& authorEmail) const {
    vector<Notice> authorNotices = getNoticesByAuthor(authorEmail);

    return any_of(authorNotices.begin(), authorNotices.end(),
                  [noticeID](const Notice& notice) {
                      return notice.getNoticeID() == noticeID;
                  });
}

// Analytics
size_t NoticeManager::getTotalNoticesCount() const {
    return getAllNotices().size();
}

size_t NoticeManager::getActiveNoticesCount() const {
    return getActiveNotices().size();
}

size_t NoticeManager::getNoticeCountByType(NoticeType type) const {
    return getNoticesByType(type).size();
}

vector<pair<NoticeType, size_t>> NoticeManager::getNoticeStatsByType() const {
    vector<pair<NoticeType, size_t>> stats;

    // Use a loop instead of hard-coding types
    vector<NoticeType> types = {
        NoticeType::GENERAL,
        NoticeType::ACADEMIC,
        NoticeType::EVENT,
        NoticeType::PUBLIC_RELATIONS,
        NoticeType::EMERGENCY
    };

    stats.reserve(types.size());
    for (const auto& type : types) {
        stats.emplace_back(type, getNoticeCountByType(type));
    }

    return stats;
}

// Additional methods for specific use cases
vector<Notice> NoticeManager::getMyPublishedNotices() const {
    // This should be overridden by derived classes with proper author identification
    return getAllNotices();
}

// Enhanced methods for new Notice functionality
vector<Notice> NoticeManager::getNoticesByDepartment(department dept) const {
    return filterNotices([dept](const Notice& notice) {
        return notice.getTargetDepartment() == dept;
    });
}

void NoticeManager::displayNoticesByType(NoticeType type) const {
    vector<Notice> notices = getNoticesByType(type);

    cout << "\n=== " << NoticeTypeHelper::toString(type) << " Notices ===" << endl;
    if (notices.empty()) {
        cout << "No notices found for this type." << endl;
        return;
    }

    for (const auto& notice : notices) {
        notice.displayNotice();
        cout << "------------------------" << endl;
    }
}

void NoticeManager::displayAllNoticeTypes() const {
    cout << "\nAvailable Notice Types:" << endl;
    vector<string> types = NoticeTypeHelper::getAllNoticeTypes();

    for (size_t i = 0; i < types.size(); ++i) {
        cout << (i + 1) << ". " << types[i] << endl;
    }
}

bool NoticeManager::createNoticeWithType(const string& title, const string& description,
                                        const string& noticeTypeStr, department targetDept) {
    if (!NoticeTypeHelper::isValidNoticeType(noticeTypeStr)) {
        cout << "Error: Invalid notice type '" << noticeTypeStr << "'" << endl;
        displayAllNoticeTypes();
        return false;
    }

    NoticeType type = NoticeTypeHelper::fromString(noticeTypeStr);
    Notice newNotice(title, description, type);
    newNotice.setTargetDepartment(targetDept);

    return saveNotice(newNotice);
}
