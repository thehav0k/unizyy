#include "date.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <filesystem>

using namespace std;

// Constructors
Date::Date() {
    time_t now = time(0);
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

// Operators
bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
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
// token er date expiry check korar jnno
bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}

bool Date::operator>=(const Date& other) const {
    return *this > other || *this == other;
}

// Static members for simulation
bool Date::simulationActive = false;
Date Date::simulatedDate = Date();
int Date::simulatedHour = -1;

// Static functions
Date Date::getCurrentDate() {
    if (simulationActive) return simulatedDate;
    return Date();
}

Date Date::getTomorrowDate() {
    Date today = getCurrentDate();
    return today.getNextDay();
}

Date Date::SimulateDate(size_t n) {
    if (!simulationActive) {
        simulatedDate = getCurrentDate();
        time_t now = time(0); simulatedHour = localtime(&now)->tm_hour;
        simulationActive = true;
    }
    for (size_t i = 0; i < n; i++) simulatedDate = simulatedDate.getNextDay();
    saveSimulationState();
    return simulatedDate;
}

Date Date::SimulateMonths(size_t n) {
    if (!simulationActive) {
        simulatedDate = getCurrentDate();
        time_t now = time(0); simulatedHour = localtime(&now)->tm_hour;
        simulationActive = true;
    }
    int totalMonths = simulatedDate.getMonth() - 1 + static_cast<int>(n);
    int newYear = simulatedDate.getYear() + totalMonths / 12;
    int newMonth = totalMonths % 12 + 1;
    int newDay = simulatedDate.getDay();
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (newMonth == 2 && ((newYear % 4 == 0 && newYear % 100 != 0) || (newYear % 400 == 0))) daysInMonth[1] = 29;
    if (newDay > daysInMonth[newMonth-1]) newDay = daysInMonth[newMonth-1];
    simulatedDate = Date(newDay, newMonth, newYear);
    saveSimulationState();
    return simulatedDate;
}

void Date::SimulateHours(size_t n) {
    if (!simulationActive) {
        simulatedDate = getCurrentDate();
        time_t now = time(0); simulatedHour = localtime(&now)->tm_hour;
        simulationActive = true;
    }
    size_t total = simulatedHour + n;
    size_t addDays = total / 24;
    simulatedHour = static_cast<int>(total % 24);
    for (size_t i = 0; i < addDays; ++i) simulatedDate = simulatedDate.getNextDay();
    saveSimulationState();
}

void Date::setSimulatedDate(const Date& d) {
    simulationActive = true;
    simulatedDate = d;
    time_t now = time(0); simulatedHour = localtime(&now)->tm_hour;
    saveSimulationState();
}

void Date::setSimulatedDateTime(const Date& d, int hour) {
    simulationActive = true;
    simulatedDate = d;
    simulatedHour = hour < 0 ? 0 : (hour > 23 ? 23 : hour);
    saveSimulationState();
}

int Date::getSimulatedHour() {
    if (!simulationActive) return -1;
    return simulatedHour;
}

bool Date::isSimulationActive() { return simulationActive; }

void Date::resetSimulation() {
    simulationActive = false;
    simulatedHour = -1;
    saveSimulationState();
}

bool Date::saveSimulationState() {
    try {
        filesystem::create_directories("Database");
        ofstream out("Database/simulation_state.dat", ios::binary | ios::trunc);
        if (!out.is_open()) return false;
        out.write(reinterpret_cast<const char*>(&simulationActive), sizeof(simulationActive));
        out.write(reinterpret_cast<const char*>(&simulatedDate), sizeof(simulatedDate));
        out.write(reinterpret_cast<const char*>(&simulatedHour), sizeof(simulatedHour));
        return true;
    } catch (...) { return false; }
}

bool Date::loadSimulationState() {
    ifstream in("Database/simulation_state.dat", ios::binary);
    if (!in.is_open()) return false;
    bool active; Date d; int hour;
    if (!in.read(reinterpret_cast<char*>(&active), sizeof(active))) return false;
    if (!in.read(reinterpret_cast<char*>(&d), sizeof(d))) return false;
    if (!in.read(reinterpret_cast<char*>(&hour), sizeof(hour))) return false;
    simulationActive = active;
    simulatedDate = d;
    simulatedHour = hour;
    return true;
}
