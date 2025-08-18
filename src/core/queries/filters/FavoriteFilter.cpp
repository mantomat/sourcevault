#include "FavoriteFilter.h"

namespace Core::Queries::Filters {

auto FavoriteFilter::clone() const -> std::unique_ptr<Filter> {
    return std::make_unique<FavoriteFilter>(*this);
}

auto FavoriteFilter::matches(const Medium *medium) const -> bool {
    return medium != nullptr && medium->userData().favorite();
}

}