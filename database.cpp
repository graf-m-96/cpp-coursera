#include "database.h"
#include "date.h"

#include <algorithm>

using namespace std;

void Database::Add(const Date& date, const string& event) {
    if (_date_to_events_for_find[date].count(event) == 0) {
        _date_to_ordered_events[date].push_back(event);
        _date_to_events_for_find[date].insert(event);
    }
}

void Database::Print(ostream& os) const {
    for (auto& [date, events] : _date_to_ordered_events) {
        for (auto& event : events) {
            os << date << ' ' << event << endl;
        }
    }
}

int Database::RemoveIf(function<bool (const Date& date, const string& event)> predicate) {
    int counter = 0;
    for (auto it_map = _date_to_ordered_events.begin(); it_map != _date_to_ordered_events.end();) {
        auto& date = it_map->first;
        auto& events = it_map->second;
        auto it_vector = remove_if(events.begin(), events.end(), [&predicate, &date, this](const string& currentEvent) {
            auto was_deleted = predicate(date, currentEvent);
            if (was_deleted) {
                _date_to_events_for_find[date].erase(currentEvent);
                if (_date_to_events_for_find[date].empty()) {
                    _date_to_events_for_find.erase(date);
                }
            }
            return was_deleted;
        });
        int new_size = it_vector - events.begin();
        int deleted = events.end() - it_vector;
        events.resize(new_size);
        counter += deleted;

        ++it_map;

        if (new_size == 0) {
            _date_to_ordered_events.erase(date);
        }
    }
    return counter;
}

vector<pair<Date, string>> Database::FindIf(function<bool (const Date& date, const string& event)> predicate) const {
    vector<pair<Date, string>> answer;
    for (auto& [date, events] : _date_to_ordered_events) {
        for (auto& event : events) {
            if (predicate(date, event)) {
                answer.push_back({date, event});
            }
        }
    }
    return answer;
}

string Database::Last(const Date& date) const {
    auto [lower, upper] = _date_to_ordered_events.equal_range(date);

    if (lower != upper) {
        return (*lower).first.to_string() + " " + (*lower).second.back();
    }

    if (lower == _date_to_ordered_events.begin()) {
        return "No entries";
    }
    --lower;

    return (*lower).first.to_string() + " " + (*lower).second.back();
}
