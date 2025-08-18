#ifndef SORT_H
#define SORT_H

#include "model/Medium.h"
#include "shared/Cloneable.h"

#include <vector>

using Core::Model::Medium;
using Core::Shared::Cloneable;

namespace Core::Queries::Sortings {

class Sort : public Cloneable<Sort> {

    bool ascending;

  public:
    ~Sort() override = default;
    explicit Sort(bool isAscending);
    Sort(const Sort &) = default;
    Sort(Sort &&) = default;
    auto operator=(const Sort &) -> Sort & = default;
    auto operator=(Sort &&) -> Sort & = default;

    [[nodiscard]] virtual auto apply(std::vector<const Medium *> media) const
        -> std::vector<const Medium *> = 0;

    [[nodiscard]] auto isAscending() const -> bool;
};

}

#endif