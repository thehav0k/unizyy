#ifndef Notice_h
#define Notice_h

#include<vector>
#include<string>
#include<date.h>
using namespace std;

class Notice {
    string title;
    string  message;
    string author;
    Date date;

public:

    Notice() {}

    Notice(string noticeTile, string msg,const Date& d);
    //getters
    string getTitle() const;
    string getMessage() const;
    Date getDate() const;
    //setters
    void setTitle(std::string newTitle);
    void setMessage(std::string newContent);
    void setDate(const Date& newDate);


};
#endif
