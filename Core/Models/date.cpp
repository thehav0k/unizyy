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
    // Parse DD-MM-YYYY format
    stringstream ss(dateStr);
    string token;

    getline(ss, token, '-');
    day = stoi(token);

    getline(ss, token, '-');
    month = stoi(token);

    getline(ss, token);
    year = stoi(token);
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
    Date nextDay = *this;
    nextDay.day++;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Check for leap year
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        daysInMonth[1] = 29;
    }

    if (nextDay.day > daysInMonth[month - 1]) {
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

        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        // Check for leap year
        if (prevDay.month == 2 && ((prevDay.year % 4 == 0 && prevDay.year % 100 != 0) || (prevDay.year % 400 == 0))) {
            daysInMonth[1] = 29;
        }

        prevDay.day = daysInMonth[prevDay.month - 1];
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

Date Date::SimulateDate(size_t n) {
    if (!isSimulating) {
        simulatedDate = Date();
        isSimulating = true;
    }
    //negative er jonno int banailam
    int days= static_cast<int>(n);
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

Date Date::SimulateMonths(size_t n) {
    if (!isSimulating) {
        simulatedDate = Date();
        isSimulating = true;
    }

    simulatedDate.month += static_cast<int>(n);

    while (simulatedDate.month > 12) {
        simulatedDate.month -= 12;
        simulatedDate.year++;
    }
    //negative month dile jate kaj kore tai add disi
    while (simulatedDate.month < 1) {
        simulatedDate.month += 12;
        simulatedDate.year--;
    }

    return simulatedDate;
}

void Date::SimulateHours(size_t n) {
    if (simulatedHour == -1) {
        time_t now = time(nullptr);
        tm* ltm = localtime(&now);
        simulatedHour = ltm->tm_hour;
    }

    simulatedHour += static_cast<int>(n);

    while (simulatedHour >= 24) {
        simulatedHour -= 24;
        SimulateDate(1); // ekdin shamne gelo
    }
    // negative hoile ki hobe?
    while (simulatedHour < 0) {
        simulatedHour += 24;
        SimulateDate(static_cast<size_t>(-1)); // ekdin pechone jabe
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
    ss << currentDate.year << "-"
       << setfill('0') << setw(2) << currentDate.month << "-"
       << setfill('0') << setw(2) << currentDate.day << " "
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
    ss << currentDate.year << "-"
       << setfill('0') << setw(2) << currentDate.month << "-"
       << setfill('0') << setw(2) << currentDate.day << " "
       << setfill('0') << setw(2) << currentHour << ":"
       << setfill('0') << setw(2) << currentMinute << ":"
       << setfill('0') << setw(2) << currentSecond;
    return ss.str();
}
