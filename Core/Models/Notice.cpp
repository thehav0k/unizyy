#include "Notice.h"
#include "../Utils/StringHelper.h"
#include <iostream>
using namespace std;

Notice::Notice(string noticeTitle, string msg, const Date &d)
    : date(d) {
    StringHelper::stringToCharArray(noticeTitle, this->title);
    StringHelper::stringToCharArray(msg, this->message);
}

// Getters
string Notice::getTitle() const {
    return StringHelper::charArrayToString(title);
}

string Notice::getMessage() const {
    return StringHelper::charArrayToString(message);
}

Date Notice::getDate() const {
    return date;
}

// Setters
void Notice::setTitle(string newTitle) {
    StringHelper::stringToCharArray(newTitle, this->title);
}

void Notice::setMessage(string newmsg) {
    StringHelper::stringToCharArray(newmsg, this->message);
}

void Notice::setDate(const Date &newDate) {
    date = newDate;
}
