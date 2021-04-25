#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

const int YEAR_TO_DAYS = 365;
const int MONTH_TO_DAYS = 31;

class Date {
public:
    void update(int new_year, int new_month, int new_day) {
        year = new_year;
        month = new_month;
        day = new_day;
        validate();
    }

    int GetYear() const {
        return year;
    }

    int GetMonth() const {
        return month;
    }

    int GetDay() const {
        return day;
    }

    int dateInDays() const {
        return YEAR_TO_DAYS * year + MONTH_TO_DAYS * month + day;
    }

private:
    int year;
    int month;
    int day;

    void validate() {
        if (month > 12 || month < 1) {
            throw runtime_error("Month value is invalid: " + to_string(month));
        }
        if (day > 31 || day < 1) {
            throw runtime_error("Day value is invalid: " + to_string(day));
        }
    }
};

bool operator < (const Date& lhs, const Date& rhs) {
    return lhs.dateInDays() < rhs.dateInDays();
}

void throwWrongDateFormat(const string& dateInStr) {
    throw runtime_error("Wrong date format: " + dateInStr);
}

void checkNextCharIsDashAndSkip(stringstream& ss) {
    if (ss.peek() != '-') {
        throwWrongDateFormat(ss.str());
    }
    ss.ignore(1);
}

istream& operator >> (stringstream& ss, Date& date) {
    int year, month, day;
    if (!(ss >> year)) {
        throwWrongDateFormat(ss.str());
    }
    checkNextCharIsDashAndSkip(ss);
    if (!(ss >> month)) {
        throwWrongDateFormat(ss.str());
    }
    checkNextCharIsDashAndSkip(ss);
    if (!(ss >> day)) {
        throwWrongDateFormat(ss.str());
    }
    string rest;
    if (ss >> rest) {
        throwWrongDateFormat(ss.str());
    }
    date.update(year, month, day);

    return ss;
}

ostream& operator << (ostream& stream, const Date& date) {
    stream << setw(4) << setfill('0') << date.GetYear() << '-'
           << setw(2) << setfill('0') << date.GetMonth() << '-'
           << setw(2) << setfill('0') << date.GetDay();
    return stream;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        dateToEvents[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const string& event) {
        if (dateToEvents.count(date) != 0) {
            if (dateToEvents[date].count(event) != 0) {
                dateToEvents[date].erase(event);
                return true;
            }
            return false;
        }
        return false;
    }

    int DeleteDate(const Date& date) {
        if (dateToEvents.count(date) != 0) {
            int n = dateToEvents[date].size();
            dateToEvents.erase(date);
            return n;
        }
        return 0;
    }

    void Find(const Date& date) const {
        if (dateToEvents.count(date) == 0) {
            return;
        }
        for (auto& event : dateToEvents.at(date)) {
            cout << event << endl;
        }
    }

    void Print() const {
        for (auto& [date, events] : dateToEvents) {
            for (auto& event : events) {
                cout << date << ' ' << event << endl;
            }
        }
    }

private:
    map<Date, set<string>> dateToEvents;
};

Date readDateFromCommand(stringstream& ssCommand) {
    string dateInstr;
    ssCommand >> dateInstr;
    stringstream ssDate(dateInstr);
    Date date;
    ssDate >> date;
    return date;
}

int main() {
    try {
        Database db;

        string command;
        while (getline(cin, command)) {
            stringstream ssCommand(command);
            string commandName;
            if (ssCommand >> commandName) {
                if (commandName == "Add") {
                    Date date = readDateFromCommand(ssCommand);
                    string event;
                    ssCommand >> event;
                    db.AddEvent(date, event);
                } else if (commandName == "Del") {
                    Date date = readDateFromCommand(ssCommand);
                    string event;
                    if (ssCommand >> event) {
                        cout << (
                                db.DeleteEvent(date, event)
                                ? "Deleted successfully"
                                : "Event not found"
                        )
                             << endl;
                    } else {
                        cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
                    }
                } else if (commandName == "Find") {
                    Date date = readDateFromCommand(ssCommand);
                    db.Find(date);
                } else if (commandName == "Print") {
                    db.Print();
                } else {
                    cout << "Unknown command: " << commandName << endl;
                }
            }
        }
    } catch (runtime_error& err) {
        cout << err.what() << endl;
    }

    return 0;
}
