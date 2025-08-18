#ifndef FILTER_H
#define FILTER_H

#include "model/Medium.h"
#include "shared/Cloneable.h"

using Core::Model::Medium;
using Core::Shared::Cloneable;

namespace Core::Queries::Filters {

/**
 * @brief Interface for a `Filter query` over a library.
 */
class Filter : public Cloneable<Filter> {

  public:
    ~Filter() override = default;
    Filter() = default;
    Filter(const Filter &) = default;
    Filter(Filter &&) = default;
    auto operator=(const Filter &) -> Filter & = default;
    auto operator=(Filter &&) -> Filter & = default;

    [[nodiscard]] virtual auto matches(const Medium *medium) const -> bool = 0;
};

}

#endif