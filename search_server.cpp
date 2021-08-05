#include "search_server.h"
#include "iterator_range.h"
#include "parse.h"
#include "profile.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <unordered_map>

void InvertedIndex::Add(string document) {
    docs.push_back(move(document));
    size_t docid = docs.size() - 1;
    for (string_view word : SplitIntoWords(docs.back())) {
        auto& docids = index[word];
        if (!docids.empty() && docids.back().first == docid) {
            ++docids.back().second;
        } else {
            docids.push_back({docid, 1});
        }
    }
}

const vector<pair<size_t, size_t>> mock_vector;

const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(string_view word) const {
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    } else {
        return mock_vector;
    }
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBaseSingle(document_input);
}

void SearchServer::UpdateDocumentBaseSingle(istream& document_input) {
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document);) {
        new_index.Add(move(current_document));
    }

    auto access = invert_index.GetAccess();
    swap(access.ref_to_value, new_index);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    requests.push_back(async([this, &document_input]() {
        UpdateDocumentBaseSingle(document_input);
    }));
}

void SearchServer::AddQueriesStreamSingle(istream& query_input, ostream& search_results_output) {
    vector<size_t> docid_to_value;
    vector<int64_t> docs_indexes;
    for (string current_query; getline(query_input, current_query);) {
        const auto words = SplitIntoWords(current_query);

        {
            auto access = invert_index.GetAccess();

            size_t docs_count = access.ref_to_value.GetDocsCount();
            docid_to_value.assign(docs_count, 0);
            docs_indexes.resize(docs_count);
            iota(docs_indexes.begin(), docs_indexes.end(), 0);

            for (auto& word : words) {
                for (auto& [docid, count] : access.ref_to_value.Lookup(word)) {
                    docid_to_value[docid] += count;
                }
            }
        }

        partial_sort(
            docs_indexes.begin(),
            Head(docs_indexes, 5).end(),
            docs_indexes.end(),
            [&docid_to_value](int64_t lhs, int64_t rhs) {
                return pair(docid_to_value[lhs], -lhs) > pair(docid_to_value[rhs], -rhs);
            });

        search_results_output << current_query << ':';
        for (int64_t docid : Head(docs_indexes, 5)) {
            size_t hitcount = docid_to_value[docid];
            if (hitcount == 0) {
                break;
            }
            search_results_output << " {"
                                  << "docid: " << docid << ", "
                                  << "hitcount: " << hitcount << '}';
        }
        search_results_output << endl;
    }
}

void SearchServer::AddQueriesStream(
    istream& query_input, ostream& search_results_output) {
    requests.push_back(async([this, &query_input, &search_results_output]() {
        AddQueriesStreamSingle(query_input, search_results_output);
    }));
}
