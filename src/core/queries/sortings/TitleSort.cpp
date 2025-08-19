#include "TitleSort.h"

namespace Core::Queries::Sortings {

TitleSort::TitleSort(bool isAscending)
    : Sort{isAscending} {}

auto TitleSort::clone() const -> std::unique_ptr<Sort> {
    return std::make_unique<TitleSort>(*this);
}

auto TitleSort::apply(std::vector<const Medium *> media) const -> std::vector<const Medium *> {
    std::ranges::sort(media, [this](const Medium *m1, const Medium *m2) {
        if (m1 == nullptr) {
            return false;
        }
        if (m2 == nullptr) {
            return true;
        }
        return isAscending() ? m1->title() < m2->title() : m1->title() > m2->title();
    });
    return media;
}

}