//
// Created by Md. Asif Khan on 15/8/25.
//

#include "NoticeManager.h"
#include <algorithm>

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
        if (notice.getNoticeID() == id && notice.getAuthorEmail() == authorEmail) {
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
        if (it->getNoticeID() == id && it->getAuthorEmail() == authorEmail) {
            allNotices.erase(it);
            DatabaseManager::saveObjects(allNotices, noticeDbFile);
            return true;
        } else {
            ++it;
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
    return filterNotices([&authorEmail](const Notice& notice) {
        return notice.getAuthorEmail() == authorEmail;
    });
}

vector<Notice> NoticeManager::getNoticesByType(NoticeType type) const {
    return filterNotices([type](const Notice& notice) {
        return notice.getNoticeType() == type;
    });
}

vector<Notice> NoticeManager::getActiveNotices() const {
    return filterNotices([](const Notice& notice) {
        return notice.getIsActive();
    });
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
