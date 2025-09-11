#include "PrioritySort.h"

namespace Core::Queries::Sortings {

PrioritySort::PrioritySort(bool isAscending)
    : Sort{isAscending} {}

auto PrioritySort::clone() const -> std::unique_ptr<Sort> {
    return std::make_unique<PrioritySort>(isAscending());
}

auto PrioritySort::apply(std::vector<const Medium *> media) const -> std::vector<const Medium *> {
    std::ranges::sort(media, [this](const Medium *m1, const Medium *m2) {
        if (m1 == nullptr || !m1->userData().priority().has()) {
            return false;
        }
        if (m2 == nullptr || !m2->userData().priority().has()) {
            return true;
        }
        const auto p1{underlying(m1->userData().priority().get().value())};
        const auto p2{underlying(m2->userData().priority().get().value())};
        return isAscending() ? p1 < p2 : p1 > p2;
    });
    return media;
}

auto PrioritySort::underlying(MediumUserData::PriorityLevel priority)
    -> std::underlying_type_t<MediumUserData::PriorityLevel> {
    return static_cast<std::underlying_type_t<MediumUserData::PriorityLevel>>(priority);
}

}