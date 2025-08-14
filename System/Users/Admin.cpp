//
// Created by Md. Asif Khan on 8/8/25.
//

#include "Admin.h"
#include "../../Core/Database/DatabaseManager.h"

// Static database operations
vector<Admin> Admin::loadAllAdmins() {
    return DatabaseManager::loadAdmins();
}
bool Admin::addAdminToDB(const Admin& admin) {
    return DatabaseManager::addAdmin(admin);
}
bool Admin::updateAdminInDB(const string& email, const Admin& updatedAdmin) {
    return DatabaseManager::updateAdmin(email, updatedAdmin);
}
bool Admin::deleteAdminFromDB(const string& email) {
    return DatabaseManager::deleteAdmin(email);
}
Admin* Admin::findAdminByEmail(const string& email) {
    return DatabaseManager::findAdminByEmail(email);
}
