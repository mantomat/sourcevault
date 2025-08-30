#include "SearchQuery.h"

namespace Core::Queries {

SearchQuery::SearchQuery(FilteringQuery newFilteringQuery, SearchEngine newSearchEngine)
    : filteringQuery{std::move(newFilteringQuery)}
    , searchEngine{std::move(newSearchEngine)} {}

auto SearchQuery::query(std::vector<const Medium *> media) const -> std::vector<const Medium *> {
    const auto filteredMedia{filteringQuery.query(std::move(media))};
    return searchEngine.search(filteredMedia);
}

}