#ifndef DATE_H
#define DATE_H

#include <string>
using namespace std;

class Date {
private:
    int day;
    int month;
    int year;
private:
    static bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }
    static int getDaysInMonth(int month, int year) {
        static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && isLeapYear(year)) {
            return 29;
        }
        return days[month - 1];
    }
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
    // normal date ke string e convert
    string toString() const;
    bool isValid() const;
    bool isToday() const;
    bool isTomorrow() const;
    bool isYesterday() const;
    Date getNextDay() const;
    Date getPreviousDay() const;

    // ============ Operator Overloading ===========
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;

    // Static functions
    static Date getCurrentDate();
    static Date getTomorrowDate();
    static Date SimulateDate(int n);
    static Date SimulateMonths(int n);
    static void SimulateHours(int n);  // advance hours
    static void setSimulatedDate(const Date& d);
    static void setSimulatedDateTime(const Date& d, int hour); // set date and hour
    static int getSimulatedHour();
    static void resetSimulation(); // Reset simulation to current real date/time
    static bool isSimulationActive(); // Check if simulation is currently active

    // interface e display korar jnno
    static string getCurrentTimeString();
    static string getCurrentDateTimeString();

    // ================ Friend function ====================
    //overloader extraction operator diye direct date print kora jabe
    friend ostream& operator<<(ostream& os, const Date& date);

};

#endif // DATE_H
