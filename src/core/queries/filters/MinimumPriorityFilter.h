#ifndef MINIMUMPRIORITYFILTER_H
#define MINIMUMPRIORITYFILTER_H

#include "Filter.h"
#include "model/MediumUserData.h"

using Core::Model::MediumUserData;

namespace Core::Queries::Filters {

class MinimumPriorityFilter : public Filter {

    MediumUserData::PriorityLevel minPriority;

  public:
    MinimumPriorityFilter(const MinimumPriorityFilter &) = default;

    explicit MinimumPriorityFilter(MediumUserData::PriorityLevel newMinPriority);

    MinimumPriorityFilter(MinimumPriorityFilter &&) = default;
    auto operator=(const MinimumPriorityFilter &) -> MinimumPriorityFilter & = default;
    auto operator=(MinimumPriorityFilter &&) -> MinimumPriorityFilter & = default;
    ~MinimumPriorityFilter() override = default;

    [[nodiscard]] auto clone() const -> std::unique_ptr<Filter> override;
    [[nodiscard]] auto matches(const Medium *medium) const -> bool override;
};

}

#endif