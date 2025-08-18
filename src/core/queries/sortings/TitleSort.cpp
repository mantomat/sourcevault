#include "TitleSort.h"

namespace Core::Queries::Sortings {

TitleSort::TitleSort(bool isAscending)
    : Sort{isAscending} {}

auto TitleSort::apply(std::vector<const Medium *> media) const -> std::vector<const Medium *> {
    std::ranges::sort(media, [](const Medium *m1, const Medium *m2) {
        if (m1 == nullptr) {
            return false;
        }
        if (m2 == nullptr) {
            return true;
        }
        return m1->title() > m2->title();
    });
    return media;
}

}