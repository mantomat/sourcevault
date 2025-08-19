#include "DateAddedSort.h"

namespace Core::Queries::Sortings {

DateAddedSort::DateAddedSort(bool isAscending)
    : Sort{isAscending} {}

auto DateAddedSort::clone() const -> std::unique_ptr<Sort> {
    return std::make_unique<DateAddedSort>(*this);
}

auto DateAddedSort::apply(std::vector<const Medium *> media) const -> std::vector<const Medium *> {
    std::ranges::sort(media, [this](const Medium *m1, const Medium *m2) {
        if (m1 == nullptr) {
            return false;
        }
        if (m2 == nullptr) {
            return true;
        }
        return isAscending() ? m1->dateAdded() < m2->dateAdded()
                             : m1->dateAdded() > m2->dateAdded();
    });
    return media;
}

}