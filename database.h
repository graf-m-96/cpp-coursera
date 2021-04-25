#pragma once

#include "date.h"

#include <iostream>
#include <functional>
#include <utility>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std;

class Database {
public:
    map<Date, unordered_set<string>> _date_to_events_for_find;

    void Add(const Date& date, const string& event);

    void Print(ostream& os) const;

    int RemoveIf(function<bool (const Date& date, const string& event)> predicate);

    vector<pair<Date, string>> FindIf(function<bool (const Date& date, const string& event)> predicate) const;

    string Last(const Date& date) const;

private:
    map<Date, vector<string>> _date_to_ordered_events;
};

template <class T1, class T2>
ostream& operator<<(ostream& os, const pair<T1, T2>& p) {
    os << p.first << ' ' << p.second;
    return os;
}
