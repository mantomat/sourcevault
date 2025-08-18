#include "MediumTypeFilter.h"

#include "shared/MediumTypeVisitor.h"

#include <unordered_set>

using Core::Shared::MediumTypeVisitor;

namespace Core::Queries::Filters {

void MediumTypeFilter::swap(MediumTypeFilter &other) noexcept {
    std::swap(visitor, other.visitor);
}

MediumTypeFilter::MediumTypeFilter(const MediumTypeFilter &other)
    : visitor{std::make_unique<MediumTypeVisitor>(*other.visitor)} {}

MediumTypeFilter::MediumTypeFilter(std::unordered_set<std::type_index> types)
    : visitor{std::make_unique<MediumTypeVisitor>(std::move(types))} {}

auto MediumTypeFilter::clone() const -> std::unique_ptr<Filter> {
    return std::make_unique<MediumTypeFilter>(*this);
}

auto MediumTypeFilter::matches(const Medium *medium) const -> bool {
    if (visitor == nullptr || medium == nullptr) {
        return false;
    }
    medium->accept(*visitor);
    return visitor->hasMatched();
}

}