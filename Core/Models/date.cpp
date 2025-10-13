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
    day = stoi(dateStr.substr(0, 2));
    month = stoi(dateStr.substr(3, 2));
    year = stoi(dateStr.substr(6, 4));
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
    ss << *this;  // Uses operator<<
    return ss.str();
}

bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    // Use the helper function
    int maxDays = getDaysInMonth(month, year);
    return day <= maxDays;
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
    Date nextDay = *this;
    nextDay.day++;

    // Use the helper function
    int maxDays = getDaysInMonth(month, year);

    if (nextDay.day > maxDays) {
        nextDay.day = 1;
        nextDay.month++;

        if (nextDay.month > 12) {
            nextDay.month = 1;
            nextDay.year++;
        }
    }

    return nextDay;
}

Date Date::getPreviousDay() const {
    Date prevDay = *this;
    prevDay.day--;

    if (prevDay.day < 1) {
        prevDay.month--;

        if (prevDay.month < 1) {
            prevDay.month = 12;
            prevDay.year--;
        }

        // Use the helper function
        prevDay.day = getDaysInMonth(prevDay.month, prevDay.year);
    }

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

Date Date::SimulateDate(int n) {
    if (!isSimulating) {
        simulatedDate = Date();
        isSimulating = true;
    }
    //negative er jonno int banailam
    // bro just function parameter change korlei to hoto
    int days= n;
    if (days >= 0)
    for (int i = 0; i < n; ++i) {
        simulatedDate = simulatedDate.getNextDay();
    }
    //negative value er jonno korsi
    else
        for (int i = 0; i < -n; ++i) {
            simulatedDate = simulatedDate.getPreviousDay();
        }
    return simulatedDate;
}

Date Date::SimulateMonths(int n) {
    if (!isSimulating) {
        simulatedDate = Date();
        isSimulating = true;
    }
// ektu vul chilo jan1 theke 31 mash simulate korle feb 31 hoito
    simulatedDate.month += n;

    while (simulatedDate.month > 12) {
        simulatedDate.month -= 12;
        simulatedDate.year++;
    }

    while (simulatedDate.month < 1) {
        simulatedDate.month += 12;
        simulatedDate.year--;
    }

    // Use the helper function
    int maxDays = getDaysInMonth(simulatedDate.month, simulatedDate.year);
    if (simulatedDate.day > maxDays) {
        simulatedDate.day = maxDays;
    }

    return simulatedDate;
}


void Date::SimulateHours(int n) {
    if (simulatedHour == -1) {
        time_t now = time(nullptr);
        tm* ltm = localtime(&now);
        simulatedHour = ltm->tm_hour;
    }

    simulatedHour += n;
// shala stupid size_t ke ke cast mare lol
    while (simulatedHour >= 24) {
        simulatedHour -= 24;
        SimulateDate(1); // ekdin shamne gelo
    }
    // negative hoile ki hobe?
    while (simulatedHour < 0) {
        simulatedHour += 24;
        SimulateDate(-1); // ekdin pechone jabe
    }

}

void Date::setSimulatedDate(const Date& d) {
    simulatedDate = d;
    isSimulating = true;
}

void Date::setSimulatedDateTime(const Date& d, int hour) {
    simulatedDate = d;
    simulatedHour = hour;
    isSimulating = true;
}

int Date::getSimulatedHour() {
    if (simulatedHour != -1) {
        return simulatedHour;
    }

    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    return ltm->tm_hour;
}

void Date::resetSimulation() {
    isSimulating = false;
    simulatedHour = -1;
    // simulatedDate will be ignored when isSimulating is false
}

bool Date::isSimulationActive() {
    return isSimulating;
}

// Time methods for MenuInterface
string Date::getCurrentTimeString() {
    Date currentDate = getCurrentDate();
    int currentHour = getSimulatedHour();

    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    int currentMinute = ltm->tm_min;

    stringstream ss;
    ss << currentDate.toString() << " "
       << setfill('0') << setw(2) << currentHour << ":"
       << setfill('0') << setw(2) << currentMinute;
    return ss.str();
}

string Date::getCurrentDateTimeString() {
    Date currentDate = getCurrentDate();
    int currentHour = getSimulatedHour();

    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    int currentMinute = ltm->tm_min;
    int currentSecond = ltm->tm_sec;

    stringstream ss;
    ss << currentDate.toString() << " "
       << setfill('0') << setw(2) << currentHour << ":"
       << setfill('0') << setw(2) << currentMinute << ":"
       << setfill('0') << setw(2) << currentSecond;
    return ss.str();
}

ostream& operator<<(ostream& os, const Date& date) {
    os << setfill('0') << setw(2) << date.day << "-"
       << setfill('0') << setw(2) << date.month << "-"
       << date.year;
    return os;
}