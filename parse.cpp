#include "parse.h"

string_view StripLeft(string_view s) {
    while (!s.empty() && isspace(s.front())) {
        s.remove_prefix(1);
    }
    return s;
}
string_view StripRight(string_view s) {
    while (!s.empty() && isspace(s.back())) {
        s.remove_suffix(1);
    }
    return s;
}

string_view Strip(string_view s) {
    return StripRight(StripLeft(s));
}

vector<string_view> SplitBy(string_view s, char sep) {
    vector<string_view> result;
    while (!s.empty()) {
        size_t pos = s.find(sep);
        result.push_back(s.substr(0, pos));
        s.remove_prefix(pos != s.npos ? pos + 1 : s.size());
    }
    return result;
}

vector<string_view> SplitIntoWords(string_view line) {
    line = Strip(line);
    vector<string_view> result;
    while (!line.empty()) {
        size_t pos = line.find(' ');
        result.push_back(line.substr(0, pos));
        line.remove_prefix(pos != line.npos ? pos + 1 : line.size());
        line = StripLeft(line);
    }
    return result;
}
