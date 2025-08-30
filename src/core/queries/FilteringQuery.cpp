#include "FilteringQuery.h"

#include <ranges>

namespace Core::Queries {

FilteringQuery::FilteringQuery(
    std::unordered_map<std::type_index, std::unique_ptr<const Filter>> newFilters)
    : filters{std::move(newFilters)} {}

void FilteringQuery::swap(FilteringQuery &other) noexcept {
    std::swap(filters, other.filters);
}

FilteringQuery::FilteringQuery(const FilteringQuery &other) {
    for (const auto &filter : other.filters | std::views::values) {
        // Why not typeid(*newFilter)? See sonarqube(cpp:S5279)
        const auto &filterRef{*filter};
        const std::type_index filterType{typeid(filterRef)};

        filters.try_emplace(filterType, filter->clone());
    }
}

auto FilteringQuery::operator=(const FilteringQuery &other) -> FilteringQuery & {
    if (&other != this) {
        FilteringQuery temp{other};
        swap(temp);
    }
    return *this;
}

auto FilteringQuery::create(std::vector<std::unique_ptr<const Filter>> newFilters)
    -> std::optional<FilteringQuery> {
    std::unordered_map<std::type_index, std::unique_ptr<const Filter>> filtersToSet;

    for (auto &filter : newFilters) {
        if (filter == nullptr) {
            return std::nullopt;
        }

        // Why not typeid(*newFilter)? See sonarqube(cpp:S5279)
        const Filter &filterRef{*filter};
        const std::type_index filterType{typeid(filterRef)};
        const auto [_, success]{filtersToSet.try_emplace(filterType, std::move(filter))};

        if (!success) {
            return std::nullopt;
        }
    }

    return std::make_optional<FilteringQuery>(std::move(filtersToSet));
}

auto FilteringQuery::query(std::vector<const Medium *> media) const -> std::vector<const Medium *> {
    std::erase_if(media, [this](const Medium *medium) {
        return medium == nullptr ||
               !std::ranges::all_of(filters | std::views::values,
                                    [&](const auto &filter) { return filter->matches(medium); });
    });
    return media;
}

}