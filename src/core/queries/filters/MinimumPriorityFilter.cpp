#include "MinimumPriorityFilter.h"

#include "model/MediumUserData.h"

#include <type_traits>

namespace Core::Queries::Filters {

MinimumPriorityFilter::MinimumPriorityFilter(MediumUserData::PriorityLevel newMinPriority)
    : minPriority{newMinPriority} {}

auto MinimumPriorityFilter::clone() const -> std::unique_ptr<Filter> {
    return std::make_unique<MinimumPriorityFilter>(minPriority);
}

auto MinimumPriorityFilter::matches(const Medium *medium) const -> bool {
    if (medium == nullptr || !medium->userData().priority().has()) {
        return false;
    }
    return std::underlying_type_t<MediumUserData::PriorityLevel>(minPriority) <=
           std::underlying_type_t<MediumUserData::PriorityLevel>(
               medium->userData().priority().get().value());
}

}