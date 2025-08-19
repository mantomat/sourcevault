#ifndef DATEADDEDSORT_H
#define DATEADDEDSORT_H

#include "queries/sortings/Sort.h"

namespace Core::Queries::Sortings {

class DateAddedSort : public Sort {

  public:
    ~DateAddedSort() override = default;
    explicit DateAddedSort(bool isAscending = true);
    DateAddedSort(const DateAddedSort &) = default;
    DateAddedSort(DateAddedSort &&) = default;
    auto operator=(const DateAddedSort &) -> DateAddedSort & = default;
    auto operator=(DateAddedSort &&) -> DateAddedSort & = default;

    [[nodiscard]] auto clone() const -> std::unique_ptr<Sort> override;

    [[nodiscard]] auto apply(std::vector<const Medium *> media) const
        -> std::vector<const Medium *> override;
};

}

#endif