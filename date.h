#pragma once

#include <iostream>

using namespace std;

class Date {
public:
    const int Year;
    const int Month;
    const int Day;

    Date(int year, int month, int day);

    string to_string() const;
};

Date ParseDate(istream& is);

ostream& operator<<(ostream& os, const Date& date);

bool operator<(const Date& a, const Date& b);
