//
// Created by Md. Asif Khan on 15/8/25.
//

#ifndef UNIZYY_NOTICEMANAGER_H
#define UNIZYY_NOTICEMANAGER_H

#include "../../Core/Models/Notice.h"
#include "../../Core/Database/DatabaseManager.h"
#include <vector>
#include <string>
#include <functional>

class NoticeManager {
protected:
    std::string noticeDbFile;

    explicit NoticeManager(const std::string& dbFile) : noticeDbFile(dbFile) {}

public:
    // Core notice operations
    std::vector<Notice> getAllNotices() const;
    bool saveNotice(const Notice& notice) const;
    bool updateNoticeById(size_t id, const Notice& notice, const std::string& authorEmail) const;
    bool removeNoticeById(size_t id, const std::string& authorEmail) const;

    // Generic filtering methods
    std::vector<Notice> filterNotices(const std::function<bool(const Notice&)>& predicate) const;
    std::vector<Notice> getNoticesByAuthor(const std::string& authorEmail) const;
    std::vector<Notice> getNoticesByType(NoticeType type) const;
    std::vector<Notice> getActiveNotices() const;
    std::vector<Notice> getNoticesByDateRange(const Date& startDate, const Date& endDate) const;

    // Generic search method
    std::vector<Notice> searchNotices(const std::string& keyword, bool searchTitle = true, bool searchContent = true) const;

    // Utility methods
    Notice* findNoticeByID(size_t noticeID) const;
    bool isNoticeOwner(size_t noticeID, const std::string& authorEmail) const;

    // Analytics
    size_t getTotalNoticesCount() const;
    size_t getActiveNoticesCount() const;
    size_t getNoticeCountByType(NoticeType type) const;
    std::vector<std::pair<NoticeType, size_t>> getNoticeStatsByType() const;
};

#endif //UNIZYY_NOTICEMANAGER_H
