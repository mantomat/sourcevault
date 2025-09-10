#include "LibraryQueryBuilder.h"

#include "queries/SearchQuery.h"
#include "queries/SortingQuery.h"
#include "queries/filters/FavoriteFilter.h"
#include "queries/filters/MediumTypeFilter.h"
#include "queries/sortings/DateAddedSort.h"
#include "queries/sortings/TitleSort.h"

using Core::Queries::SearchQuery;
using Core::Queries::SortingQuery;
using Core::Queries::Filters::FavoriteFilter;
using Core::Queries::Filters::MediumTypeFilter;
using Core::Queries::Sortings::DateAddedSort;
using Core::Queries::Sortings::TitleSort;

namespace Gui::Components {

auto LibraryQueryBuilder::build(const LibrarySidebar::SidebarState &state,
                                const QString &searchQuery) -> std::unique_ptr<Query> {
    std::unique_ptr<Query> filter;
    if (!searchQuery.isEmpty()) {
        assert(state.sort == LibrarySidebar::SortTypeOptions::Disabled);
        return sidebarAndSearchStateToSearchQuery(state, searchQuery);
    }

    assert(state.sort != LibrarySidebar::SortTypeOptions::Disabled);
    return sidebarStateToSortingQuery(state);
}

auto LibraryQueryBuilder::sidebarAndSearchStateToSearchQuery(
    const LibrarySidebar::SidebarState &state, const QString &searchQuery)
    -> std::unique_ptr<Query> {
    FilteringQuery filtering{sidebarStateToFilteringQuery(state)};

    return std::make_unique<SearchQuery>(std::move(filtering), SearchEngine{searchQuery});
}

auto LibraryQueryBuilder::sidebarStateToSortingQuery(const LibrarySidebar::SidebarState &state)
    -> std::unique_ptr<Query> {
    FilteringQuery filtering{sidebarStateToFilteringQuery(state)};
    std::unique_ptr<Sort> sort;

    switch (state.sort) {
    case LibrarySidebar::SortTypeOptions::TitleAsc:
        sort = std::make_unique<TitleSort>(true);
        break;
    case LibrarySidebar::SortTypeOptions::TitleDesc:
        sort = std::make_unique<TitleSort>(false);
        break;
    case LibrarySidebar::SortTypeOptions::DateAddedDesc:
        sort = std::make_unique<DateAddedSort>(true);
        break;
    case LibrarySidebar::SortTypeOptions::DateAddedAsc:
        sort = std::make_unique<DateAddedSort>(false);
        break;
    case LibrarySidebar::SortTypeOptions::Disabled:
        qWarning() << "This place should never be reached";
        assert(false);
        break;
    }

    return std::make_unique<SortingQuery>(
        SortingQuery::create(std::move(filtering), std::move(sort)).value());
}

auto LibraryQueryBuilder::sidebarStateToFilteringQuery(const LibrarySidebar::SidebarState &state)
    -> FilteringQuery {
    std::vector<std::unique_ptr<const Filter>> filters;

    switch (state.mediumTypeFilter) {
    case LibrarySidebar::MediumTypeFilterOptions::Article:
        filters.push_back(std::make_unique<MediumTypeFilter>(
            std::unordered_set<std::type_index>{typeid(Article)}));
        break;
    case LibrarySidebar::MediumTypeFilterOptions::Book:
        filters.push_back(
            std::make_unique<MediumTypeFilter>(std::unordered_set<std::type_index>{typeid(Book)}));
        break;
    case LibrarySidebar::MediumTypeFilterOptions::Video:
        filters.push_back(
            std::make_unique<MediumTypeFilter>(std::unordered_set<std::type_index>{typeid(Video)}));
        break;
    case LibrarySidebar::MediumTypeFilterOptions::NoFilter:
        break;
    }

    if (state.onlyFavoriteFilter) {
        filters.push_back(std::make_unique<FavoriteFilter>());
    }

    // TODO model minimumPriorityFilter, topicsFilter

    return FilteringQuery::create(std::move(filters)).value();
}

}