#pragma once

#include "synchronized.h"

#include <future>
#include <istream>
#include <map>
#include <mutex>
#include <ostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

class InvertedIndex {
public:
    InvertedIndex() = default;

    void Add(string document);
    const vector<pair<size_t, size_t>>& Lookup(string_view word) const;

    string_view GetDocument(size_t id) const {
        return docs[id];
    }

    size_t GetDocsCount() const {
        return docs.size();
    }

private:
    map<string_view, vector<pair<size_t, size_t>>> index;
    deque<string> docs;
};

class SearchServer {
public:
    SearchServer() = default;

    explicit SearchServer(istream& document_input);
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    Synchronized<InvertedIndex> invert_index;
    vector<future<void>> requests;

    void AddQueriesStreamSingle(istream& query_input, ostream& search_results_output);
    void UpdateDocumentBaseSingle(istream& document_input);
};
