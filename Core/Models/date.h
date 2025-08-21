//
// Created by Md. Asif Khan on 10/8/25.
//

#ifndef DATE_H
#define DATE_H

#include <string>
using namespace std;

class Date {
private:
    int day;
    int month;
    int year;

public:
    // Constructors
    Date();
    Date(int d, int m, int y);
    explicit Date(const string& dateStr); // Format: DD-MM-YYYY

    // Getters
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    // Setters
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);

    // Utility functions
    //debugging er shubidha er jonno nodiscard use kora hoise, basically jodi keu container chara kono function
    //call kore jeitar return type void na, tokhon warning dekhabe
     string toString() const;           // Returns DD-MM-YYYY format
    bool isValid() const;
    bool isToday() const;
    bool isTomorrow() const;
    bool isYesterday() const;
    Date getNextDay() const;
    Date getPreviousDay() const;

    // date gula compare korar jonno operator overloading
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;

    // Static functions
    static Date getCurrentDate();
    static Date getTomorrowDate();
};

#endif // DATE_H
