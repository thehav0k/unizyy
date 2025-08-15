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
    [[nodiscard]] int getDay() const;
    [[nodiscard]] int getMonth() const;
    [[nodiscard]] int getYear() const;

    // Setters
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);

    // Utility functions
    //debugging er shubidha er jonno nodiscard use kora hoise, basically jodi keu container chara kono function
    //call kore jeitar return type void na, tokhon warning dekhabe
    [[nodiscard]] string toString() const;           // Returns DD-MM-YYYY format
    [[nodiscard]] bool isValid() const;
    [[nodiscard]] bool isToday() const;
    [[nodiscard]] bool isTomorrow() const;
    [[nodiscard]] bool isYesterday() const;
    [[nodiscard]] Date getNextDay() const;
    [[nodiscard]] Date getPreviousDay() const;

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
    static Date SimulateDate(size_t n);
    static Date SimulateMonths(size_t n); // new: advance months
    static void SimulateHours(size_t n);  // advance hours (affects meal time logic)
    static void setSimulatedDate(const Date& d);
    static void setSimulatedDateTime(const Date& d, int hour); // set date and hour
    static int getSimulatedHour();
    static void resetSimulation(); // Reset simulation to current real date/time
    static bool isSimulationActive(); // Check if simulation is currently active

    // New time methods for MenuInterface
    static string getCurrentTimeString(); // Returns YYYY-MM-DD HH:MM format (no seconds)
    static string getCurrentDateTimeString(); // Returns YYYY-MM-DD HH:MM:SS format
};

#endif // DATE_H
