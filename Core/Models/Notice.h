#ifndef Notice_h
#define Notice_h

#include<vector>
#include<string>
#include "date.h"
using namespace std;

class Notice {
private:
    char title[90];
    char message[300];
    Date date;

public:
    // Constructors
    Notice() : date() {
        title[0] = '\0';
        message[0] = '\0';
    }

    Notice(string noticeTile, string msg, const Date& d);

    // Getters
    string getTitle() const;
    string getMessage() const;
    Date getDate() const;

    // Setters
    void setTitle(string newTitle);
    void setMessage(string newContent);
    void setDate(const Date& newDate);
};

#endif