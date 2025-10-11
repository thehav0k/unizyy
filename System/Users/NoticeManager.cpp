#include "NoticeManager.h"
#include<iostream>
using namespace std;

Notice::Notice(string noticeTile, string msg,const Date &d)
:title(noticeTile),message(msg),date(d) {

}

//getters
string Notice::getTitle() const {
    return title;
}
string Notice::getMessage() const {
    return message;
}

Date Notice::getDate() const {
    return date;
}

//setters

void Notice::setTitle(string newTitle) {
    title=newTitle;
}
void Notice::setMessage(std::string newmsg) {
    message=newmsg;
}
void Notice::setDate(const Date &newDate) {
    date=newDate;
}


