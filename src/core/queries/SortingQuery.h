#ifndef SORTINGQUERY_H
#define SORTINGQUERY_H

#include "queries/FilteringQuery.h"
#include "queries/sortings/Sort.h"

#include <memory>

using Core::Queries::FilteringQuery;
using Core::Queries::Sortings::Sort;

namespace Core::Queries {

class SortingQuery final {

    FilteringQuery filteringQuery;
    std::unique_ptr<const Sort> sort;

    void swap(SortingQuery &other) noexcept;

    SortingQuery(FilteringQuery newFilteringQuery, std::unique_ptr<const Sort> newSort);

  public:
    SortingQuery() = delete;
    ~SortingQuery() = default;
    SortingQuery(SortingQuery &&) = default;
    auto operator=(SortingQuery &&) -> SortingQuery & = default;

    SortingQuery(const SortingQuery &other);
    auto operator=(const SortingQuery &other) -> SortingQuery &;

    [[nodiscard]] static auto create(FilteringQuery newFilteringQuery,
                                     std::unique_ptr<const Sort> newSort)
        -> std::optional<SortingQuery>;

    [[nodiscard]] auto query(std::vector<const Medium *> media) const
        -> std::vector<const Medium *>;
};

}

#endif