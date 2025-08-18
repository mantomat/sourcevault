#ifndef FAVORITEFILTER_H
#define FAVORITEFILTER_H

#include "queries/filters/Filter.h"

namespace Core::Queries::Filters {

class FavoriteFilter final : public Filter {
  public:
    ~FavoriteFilter() override = default;
    FavoriteFilter() = default;
    FavoriteFilter(const FavoriteFilter &) = default;
    FavoriteFilter(FavoriteFilter &&) = default;
    auto operator=(const FavoriteFilter &) -> FavoriteFilter & = default;
    auto operator=(FavoriteFilter &&) -> FavoriteFilter & = default;

    [[nodiscard]] auto clone() const -> std::unique_ptr<Filter> override;

    [[nodiscard]] auto matches(const Medium *medium) const -> bool override;
};

}

#endif