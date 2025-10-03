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
    // Expect DD-MM-YYYY
    int d = 1, m = 1, y = 1970;
    char sep1 = '-', sep2 = '-';
    stringstream ss(dateStr);
    if ((ss >> d >> sep1 >> m >> sep2 >> y) && sep1 == '-' && sep2 == '-') {
        day = d; month = m; year = y;
    } else {
        // Fallback to today on parse failure
        Date tmp; day = tmp.getDay(); month = tmp.getMonth(); year = tmp.getYear();
    }
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

static bool isLeap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dim = daysInMonth[month - 1];
    if (month == 2 && isLeap(year)) dim = 29;
    return day <= dim;
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
    int y = year, m = month, d = day;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dim = daysInMonth[m - 1];
    if (m == 2 && isLeap(y)) dim = 29;

    d += 1;
    if (d > dim) {
        d = 1;
        m += 1;
        if (m > 12) {
            m = 1;
            y += 1;
        }
    }
    return Date(d, m, y);
}

Date Date::getPreviousDay() const {
    int y = year, m = month, d = day;
    if (d > 1) {
        return Date(d - 1, m, y);
    }
    // Go to last day of previous month
    m -= 1;
    if (m < 1) {
        m = 12;
        y -= 1;
    }
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dim = daysInMonth[m - 1];
    if (m == 2 && isLeap(y)) dim = 29;
    return Date(dim, m, y);
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
    return Date();
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
