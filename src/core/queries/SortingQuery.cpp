#include "SortingQuery.h"

#include "queries/FilteringQuery.h"

namespace Core::Queries {

void SortingQuery::swap(SortingQuery &other) noexcept {
    std::swap(filteringQuery, other.filteringQuery);
    std::swap(sort, other.sort);
}

SortingQuery::SortingQuery(FilteringQuery newFilteringQuery, std::unique_ptr<const Sort> newSort)
    : filteringQuery{std::move(newFilteringQuery)}
    , sort{std::move(newSort)} {}

SortingQuery::SortingQuery(const SortingQuery &other)
    : filteringQuery{other.filteringQuery}
    , sort{other.sort->clone()} {}

auto SortingQuery::operator=(const SortingQuery &other) -> SortingQuery & {
    if (&other != this) {
        SortingQuery temp{other};
        swap(temp);
    }
    return *this;
}

auto SortingQuery::create(FilteringQuery newFilteringQuery, std::unique_ptr<const Sort> newSort)
    -> std::optional<SortingQuery> {
    if (newSort == nullptr) {
        return std::nullopt;
    }
    return SortingQuery(std::move(newFilteringQuery), std::move(newSort));
}

auto SortingQuery::query(std::vector<const Medium *> media) const -> std::vector<const Medium *> {
    auto filteredMedia{filteringQuery.query(std::move(media))};
    return sort->apply(std::move(filteredMedia));
}

}