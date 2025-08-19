#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include "model/Library.h"
#include "model/Medium.h"
#include "queries/filters/Filter.h"
#include "queries/sortings/Sort.h"

#include <typeindex>
#include <unordered_map>

using Core::Model::Library;
using Core::Model::Medium;
using Core::Queries::Filters::Filter;
using Core::Queries::Sortings::Sort;

namespace Core::Queries {

class QueryBuilder {
    std::unordered_map<std::type_index, std::unique_ptr<const Filter>> filters;
    std::unique_ptr<const Sort> sort;

    void swap(QueryBuilder &other) noexcept;

  public:
    ~QueryBuilder() = default;
    QueryBuilder() = default;
    QueryBuilder(QueryBuilder &&) = default;
    auto operator=(QueryBuilder &&) -> QueryBuilder & = default;

    /**
     * @brief Copies a QueryBuilder, performing a deep copy.
     */
    QueryBuilder(const QueryBuilder &);

    /**
     * @brief Copy-assigns a QueryBuilder, performing a deep copy.
     */
    auto operator=(const QueryBuilder &) -> QueryBuilder &;

    /**
     * @brief Adds a filter to the query pipeline. Only one filter per type is permitted.
     * Important! Any Filter with the same type will be replaced.
     * Nullptrs are ignored.
     * @return True if the pointer is valid and the filter gets added. False if the argument was
     * nullptr.
     */
    auto addFilter(std::unique_ptr<const Filter> newFilter) -> bool;

    /**
     * @brief Sets the sort for the current builder.
     */
    auto setSort(std::unique_ptr<const Sort> newSort) -> bool;

    void reset();

    /**
     * @brief Performs a query on the given library.
     */
    [[nodiscard]] auto query(const Library &library) const -> std::vector<const Medium *>;
};

}

#endif