#ifndef TITLESORT_H
#define TITLESORT_H

#include "queries/sortings/Sort.h"

namespace Core::Queries::Sortings {

class TitleSort : public Sort {

  public:
    ~TitleSort() override = default;
    explicit TitleSort(bool isAscending = true);
    TitleSort(const TitleSort &) = default;
    TitleSort(TitleSort &&) = default;
    auto operator=(const TitleSort &) -> TitleSort & = default;
    auto operator=(TitleSort &&) -> TitleSort & = default;

    [[nodiscard]] auto clone() const -> std::unique_ptr<Sort> override;

    [[nodiscard]] auto apply(std::vector<const Medium *> media) const
        -> std::vector<const Medium *> override;
};

}

#endif