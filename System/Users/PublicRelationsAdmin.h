//
// Created by Md. Asif Khan on 15/8/25.
//

#ifndef UNIZYY_PUBLICRELATIONSADMIN_H
#define UNIZYY_PUBLICRELATIONSADMIN_H

#include "user.h"
#include <vector>
#include <string>
#include <utility>

class PublicRelationsAdmin : public User {
private:
    char name[100];
    static const std::string NOTICE_DB_FILE; // text file storing PR notices
public:
    PublicRelationsAdmin();
    PublicRelationsAdmin(const std::string& email, const std::string& password, const std::string& name);

    std::string getName() const;
    void setName(const std::string& n);

    // Notice operations
    bool postNotice(const std::string& title, const std::string& body) const; // append
    std::vector<std::pair<std::string,std::string>> loadNotices() const;
    bool removeNotice(const std::string& title) const; // remove by title
    void listNotices() const; // display
};


#endif //UNIZYY_PUBLICRELATIONSADMIN_H