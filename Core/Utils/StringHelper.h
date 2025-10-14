#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <cstring>
#include <iostream>
using namespace std;
class StringHelper {
public:
    // Basically string ekta class tai shorashori object writing kora jayna file e
    // ejnno sob class e string er jaygay char array use korsi

    template<size_t N>
    static void stringToCharArray(const string& source, char (&destination)[N]) {
        strncpy(destination, source.c_str(), N - 1);
        destination[N - 1] = '\0';
    }


    // Overloaded func
    static void stringToCharArray(const string& source, char* destination, size_t maxSize) {
        strncpy(destination, source.c_str(), maxSize - 1);
        destination[maxSize - 1] = '\0';
    }


    // char array to string
    static string charArrayToString(const char* source) {
        return string(source);
    }


    // Safe string to char array
    template<size_t N>
    static bool safeStringToCharArray(const string& source, char (&destination)[N],
                                     const string& fieldName = "") {
        if (source.length() >= N) {
            if (!fieldName.empty()) {
                cout << "Warning: " << fieldName << " truncated from "
                         << source.length() << " to " << (N - 1) << " characters" << endl;
            }
        }
   // source theke n-1 ta copy korbe
        strncpy(destination, source.c_str(), N - 1);
        destination[N - 1] = '\0';
        return source.length() < N;
    }


    // Check if string fits in char array without truncation
    static bool willFitInCharArray(const string& source, size_t maxSize) {
        return source.length() < maxSize;
    }

    // Get recommended size for a string (with padding)
    static size_t getRecommendedSize(const string& source, size_t padding = 20) {
        return source.length() + padding;
    }


    // email must have @juniv.edu
    // before @ must have at least 4 characters
   static bool validateEmail(const string& email) {
        if (email.length() < 14 || email.find('@') == string::npos) {
            return false;
        }

        size_t atPos = email.find('@');
        if (atPos < 4 || email.substr(atPos) != "@juniv.edu") {
            return false; // Invalid domain or too short before '@'
        }

        return true;
    }

    // Password must be at least 6 characters long
    // must contain at least one uppercase letter, one lowercase letter, and one digit
    static bool validatePassword(const string& password) {
        if (password.length() < 6) {
            return false; // Too short
        }
        bool hasUpper = false, hasLower = false, hasDigit = false;
        for (char c : password) {
            if (isupper(c)) hasUpper = true;
            if (islower(c)) hasLower = true;
            if (isdigit(c)) hasDigit = true;
        }
        return hasUpper && hasLower && hasDigit; // Must contain at least one uppercase, one lowercase, and one digit
    }

    // kew khali or only space string inout dite parbena
    // first character space hote parbena
    static bool isValidString(const string& input) {
        if (input.empty() || input[0]==' ') return false; // Empty string
        for (const char c : input) {
            if (!isspace(c)) return true; // Contains non-space character
        }
        return false; // Only spaces
    }

    static bool isValidstudentID(const string& studentID) {
        if (studentID.length() != 11) return false;
        for (const char c : studentID) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

};

#endif //STRINGUTILS_H

// checked #1 by Asif