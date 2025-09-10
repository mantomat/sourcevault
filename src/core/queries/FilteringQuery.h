#ifndef FILTERINGQUERY_H
#define FILTERINGQUERY_H

#include "model/Medium.h"
#include "queries/Query.h"
#include "queries/filters/Filter.h"
#include "queries/search/SearchEngine.h"
#include "queries/sortings/Sort.h"

#include <memory>
#include <typeindex>
#include <unordered_map>

using Core::Model::Medium;
using Core::Queries::Filters::Filter;
using Core::Queries::Search::SearchEngine;
using Core::Queries::Sortings::Sort;

namespace Core::Queries {

class FilteringQuery final : Query {
    std::unordered_map<std::type_index, std::unique_ptr<const Filter>> filters;

    FilteringQuery(std::unordered_map<std::type_index, std::unique_ptr<const Filter>> newFilters);

    void swap(FilteringQuery &other) noexcept;

  public:
    FilteringQuery() = delete;
    ~FilteringQuery() override = default;
    FilteringQuery(FilteringQuery &&) = default;
    auto operator=(FilteringQuery &&) -> FilteringQuery & = default;

    /**
     * @brief Copies a FilteringQuery, performing a deep copy.
     */
    FilteringQuery(const FilteringQuery &other);

    /**
     * @brief Copy-assigns a FilteringQuery, performing a deep copy.
     */
    auto operator=(const FilteringQuery &) -> FilteringQuery &;

    /**
     * @brief Tries to construct a FilteringQuery.
     * @return The FilteringQuery if `newFilters` is valid (i.e. vector doesn't have nullptr's or
     * multiple filters of the same type)
     */
    [[nodiscard]] static auto create(std::vector<std::unique_ptr<const Filter>> newFilters)
        -> std::optional<FilteringQuery>;

    /**
     * @brief Filters out the given collection of media.
     */
    [[nodiscard]] auto query(std::vector<const Medium *> media) const
        -> std::vector<const Medium *> override;
};

}
#endif