#include "date.h"
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

// Static variables for simulation
static Date simulatedDate;
static bool isSimulating = false;
static int simulatedHour = -1; // -1 means use real time

// Constructors
Date::Date() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    day = ltm->tm_mday;
    month = ltm->tm_mon + 1;
    year = ltm->tm_year + 1900;
}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

Date::Date(const string& dateStr) {

}

// Getters
int Date::getDay() const { return day; }
int Date::getMonth() const { return month; }
int Date::getYear() const { return year; }

// Setters
void Date::setDay(int d) { day = d; }
void Date::setMonth(int m) { month = m; }
void Date::setYear(int y) { year = y; }

// Utility functions
string Date::toString() const {
    stringstream ss;
    ss << setfill('0') << setw(2) << day << "-"
       << setfill('0') << setw(2) << month << "-"
       << year;
    return ss.str();
}

bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Check for leap year
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        return day <= 29;
    }

    return day <= daysInMonth[month - 1];
}

bool Date::isToday() const {
    Date today = getCurrentDate();
    return *this == today;
}

bool Date::isTomorrow() const {
    Date tomorrow = getTomorrowDate();
    return *this == tomorrow;
}

bool Date::isYesterday() const {
    Date yesterday = getCurrentDate().getPreviousDay();
    return *this == yesterday;
}

Date Date::getNextDay() const {

    return nextDay;
}

Date Date::getPreviousDay() const {

    return prevDay;
}

// Comparison operators
bool Date::operator==(const Date& other) const {
    return (day == other.day && month == other.month && year == other.year);
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator>(const Date& other) const {
    return other < *this;
}

bool Date::operator<=(const Date& other) const {
    return (*this < other) || (*this == other);
}

bool Date::operator>=(const Date& other) const {
    return (*this > other) || (*this == other);
}

// Static functions
Date Date::getCurrentDate() {
    if (isSimulating) {
        return simulatedDate;
    }
    return {};
}

Date Date::getTomorrowDate() {
    Date today = getCurrentDate();
    return today.getNextDay();
}

Date Date::SimulateDate(size_t n) {
    if (!isSimulating) {
        simulatedDate = Date();
        isSimulating = true;
    }

    for (size_t i = 0; i < n; ++i) {
        simulatedDate = simulatedDate.getNextDay();
    }

    return simulatedDate;
}

