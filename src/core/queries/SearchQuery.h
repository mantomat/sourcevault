#ifndef SEARCHQUERY_H
#define SEARCHQUERY_H

#include "queries/FilteringQuery.h"
#include "queries/search/SearchEngine.h"

using Core::Queries::FilteringQuery;
using Core::Queries::Search::SearchEngine;
namespace Core::Queries {

class SearchQuery final {

    FilteringQuery filteringQuery;
    SearchEngine searchEngine;

  public:
    SearchQuery() = delete;
    ~SearchQuery() = default;
    SearchQuery(SearchQuery &&) = default;
    auto operator=(SearchQuery &&) -> SearchQuery & = default;
    SearchQuery(const SearchQuery &other) = default;
    auto operator=(const SearchQuery &other) -> SearchQuery & = default;

    SearchQuery(FilteringQuery newFilteringQuery, SearchEngine newSearchEngine);

    [[nodiscard]] auto query(std::vector<const Medium *> media) const
        -> std::vector<const Medium *>;
};

}

#endif