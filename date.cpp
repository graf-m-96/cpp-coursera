#include "date.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

Date::Date(int year, int month, int day) : Year(year), Month(month), Day(day) {}

string Date::to_string() const {
    stringstream ss;
    ss << setw(4) << setfill('0') << Year << '-'
        << setw(2) << setfill('0') << Month << '-'
        << setw(2) << setfill('0') << Day;
    return ss.str();
}

Date ParseDate(istream& is) {
    int year, month, day;
    is >> year;
    if (is.get() != '-') {
        throw logic_error("Uncorrect format date");
    }

    is >> month;
    if (is.get() != '-') {
        throw logic_error("Uncorrect format date");
    }

    is >> day;

    return Date(year, month, day);
}

ostream& operator<<(ostream& os, const Date& date) {
    os << setw(4) << setfill('0') << date.Year << '-'
        << setw(2) << setfill('0') << date.Month << '-'
        << setw(2) << setfill('0') << date.Day;

    return os;
}

bool operator<(const Date& a, const Date& b) {
    const auto a_tuple = tie(a.Year, a.Month, a.Day);
    const auto b_tuple = tie(b.Year, b.Month, b.Day);
    return a_tuple < b_tuple;
}

