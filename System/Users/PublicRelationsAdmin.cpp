//
// Created by Md. Asif Khan on 15/8/25.
//

#include "PublicRelationsAdmin.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "../../Core/Utils/StringHelper.h"
#include "../../Core/Models/date.h"

using namespace std;

const std::string PublicRelationsAdmin::NOTICE_DB_FILE = "Database/pr_notices.txt";

PublicRelationsAdmin::PublicRelationsAdmin() : User() { name[0] = '\0'; }

PublicRelationsAdmin::PublicRelationsAdmin(const std::string& email, const std::string& password, const std::string& nm)
    : User(email, password) {
    StringHelper::stringToCharArray(nm, name);
}

std::string PublicRelationsAdmin::getName() const { return StringHelper::charArrayToString(name); }
void PublicRelationsAdmin::setName(const std::string& n) { StringHelper::stringToCharArray(n, name); }

bool PublicRelationsAdmin::postNotice(const std::string& title, const std::string& body) const {
    try {
        filesystem::create_directories("Database");
        ofstream out(NOTICE_DB_FILE, ios::app);
        if (!out.is_open()) return false;
        Date today = Date::getCurrentDate();
        out << title << "|" << body << "|" << today.toString() << "\n";
        return true;
    } catch (...) { return false; }
}

vector<pair<string,string>> PublicRelationsAdmin::loadNotices() const {
    vector<pair<string,string>> notices;
    ifstream in(NOTICE_DB_FILE);
    if (!in.is_open()) return notices;
    string line;
    while (getline(in, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.rfind('|');
        if (p1==string::npos || p2==string::npos || p1==p2) continue;
        string title = line.substr(0,p1);
        string body = line.substr(p1+1, p2-p1-1);
        // date = line.substr(p2+1) (unused for now)
        notices.emplace_back(title, body);
    }
    return notices;
}

bool PublicRelationsAdmin::removeNotice(const std::string& title) const {
    ifstream in(NOTICE_DB_FILE);
    if (!in.is_open()) return false;
    vector<string> lines; string line; bool removed=false;
    while (getline(in,line)) {
        size_t p = line.find('|');
        if (p!=string::npos) {
            string lt = line.substr(0,p);
            if (lt == title) { removed=true; continue; }
        }
        lines.push_back(line);
    }
    in.close();
    if (!removed) return false;
    ofstream out(NOTICE_DB_FILE, ios::trunc);
    for (auto& l: lines) out << l << '\n';
    return true;
}

void PublicRelationsAdmin::listNotices() const {
    auto notices = loadNotices();
    if (notices.empty()) {
        cout << "No PR notices found." << endl;
        return;
    }
    cout << "\n=== PUBLIC RELATIONS NOTICES ===" << endl;
    for (size_t i=0;i<notices.size();++i) {
        cout << i+1 << ". " << notices[i].first << "\n" << notices[i].second << "\n---" << endl;
    }
}
