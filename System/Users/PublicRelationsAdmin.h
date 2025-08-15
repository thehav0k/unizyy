//
// Created by Md. Asif Khan on 15/8/25.
//

#ifndef UNIZYY_PUBLICRELATIONSADMIN_H
#define UNIZYY_PUBLICRELATIONSADMIN_H

#include "user.h"
#include "NoticeManager.h"
#include "../../Core/Models/Notice.h"
#include "../../Core/Database/DatabaseManager.h"
#include <vector>
#include <string>
#include <utility>

class PublicRelationsAdmin final : public User, public NoticeManager {
private:
    char name[100];

public:
    // Constructors
    PublicRelationsAdmin();
    PublicRelationsAdmin(const std::string& email, const std::string& password, const std::string& name);

    // Basic getters and setters
    std::string getName() const;
    void setName(const std::string& n);

    // Virtual methods from User
    void display() const override;

    // Notice creation methods (simplified)
    Notice createNotice(const std::string& title, const std::string& description, NoticeType type = NoticeType::GENERAL) const;
    Notice createPublicAnnouncement(const std::string& title, const std::string& description) const;

    // Notice management operations (simplified)
    bool publishNotice(const Notice& notice) const;
    bool publishNotice(const std::string& title, const std::string& body) const;

    // Convenience methods that wrap base class functionality
    std::vector<Notice> getMyPublishedNotices() const { return getNoticesByAuthor(getEmail()); }
    bool withdrawNotice(size_t noticeID) const { return removeNoticeById(noticeID, getEmail()); }
    bool updateNotice(size_t noticeID, const Notice& updatedNotice) const { return updateNoticeById(noticeID, updatedNotice, getEmail()); }
    bool isMyNotice(size_t noticeID) const { return isNoticeOwner(noticeID, getEmail()); }

    // Bulk operations
    bool publishMultipleNotices(const std::vector<Notice>& notices) const;
    bool archiveOldNotices(const Date& cutoffDate) const;

    // Enhanced search and filtering
    std::vector<Notice> getNoticesForDepartment(department dept) const;
    void displayNoticeStatistics() const;

    // Static database operations
    static std::vector<PublicRelationsAdmin> loadAllPRAdmins();
    static bool addPRAdminToDB(const PublicRelationsAdmin& admin);
    static bool updatePRAdminInDB(const std::string& email, const PublicRelationsAdmin& updatedAdmin);
    static bool deletePRAdminFromDB(const std::string& email);
    static PublicRelationsAdmin* findPRAdminByEmail(const std::string& email);
};

#endif //UNIZYY_PUBLICRELATIONSADMIN_H