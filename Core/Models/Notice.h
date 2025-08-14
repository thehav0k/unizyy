//
// Created by Md. Asif Khan on 15/8/25.
//

#ifndef UNIZYY_NOTICE_H
#define UNIZYY_NOTICE_H
#include "date.h"
#include "department.h"


class Notice {
protected:
    Date date;
    char title[100];
    char description[300];
    // signed by who variable
public:
    Notice();
    Notice(const Date& date, const string& description);
    Date getDate() const;
    string getDescription() const;
    void setDate(const Date& date);
    void setDescription(const string& description);
    void displayNotice() const=0;





};


#endif //UNIZYY_NOTICE_H