#include "QueryBuilder.h"

#include "queries/search/SearchEngine.h"

#include <ranges>

namespace Core::Queries {

void QueryBuilder::swap(QueryBuilder &other) noexcept {
    std::swap(filters, other.filters);
    std::swap(sort, other.sort);
}

QueryBuilder::QueryBuilder(const QueryBuilder &other)
    : sort{other.sort != nullptr ? other.sort->clone() : nullptr} {
    for (const auto &filter : other.filters | std::views::values) {
        // Why not typeid(*newFilter)? See sonarqube(cpp:S5279)
        const auto &filterRef{*filter};
        const std::type_index filterType{typeid(filterRef)};

        filters.try_emplace(filterType, filter->clone());
    }
}

auto QueryBuilder::operator=(const QueryBuilder &other) -> QueryBuilder & {
    QueryBuilder copy{other};
    swap(copy);
    return *this;
}

auto QueryBuilder::addFilter(std::unique_ptr<const Filter> newFilter) -> bool {
    if (newFilter == nullptr) {
        return false;
    }

    // Why not typeid(*newFilter)? See sonarqube(cpp:S5279)
    const auto &filterRef{*newFilter};
    const std::type_index filterType{typeid(filterRef)};
    filters[filterType] = std::move(newFilter);
    return true;
}

auto QueryBuilder::setSort(std::unique_ptr<const Sort> newSort) -> bool {
    if (newSort == nullptr) {
        return false;
    }
    searchEngine = nullptr;
    sort = std::move(newSort);
    return true;
}

auto QueryBuilder::setSearch(std::unique_ptr<const SearchEngine> newSearchEngine) -> bool {
    if (newSearchEngine == nullptr) {
        return false;
    }
    sort = nullptr;
    searchEngine = std::move(newSearchEngine);
    return true;
}

auto QueryBuilder::query(const Library &library) const -> std::vector<const Medium *> {
    const auto &media{library.getMedia()};

    auto mediaView{media | std::views::filter([this](const Medium *medium) {
                       return std::ranges::all_of(
                           filters | std::views::values,
                           [&](const auto &filter) { return filter->matches(medium); });
                   })};
    std::vector<const Medium *> filteredMedia{mediaView.begin(), mediaView.end()};

    if (sort != nullptr) {
        return sort->apply(std::move(filteredMedia));
    }
    if (searchEngine != nullptr) {
        return searchEngine->search(filteredMedia);
    }
    return filteredMedia;
}

void QueryBuilder::reset() {
    filters.clear();
    sort = nullptr;
}

}