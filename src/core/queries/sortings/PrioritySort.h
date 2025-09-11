#ifndef PRIORITYSORT_H
#define PRIORITYSORT_H

#include "Sort.h"
#include "model/MediumUserData.h"

#include <type_traits>

using Core::Model::MediumUserData;

namespace Core::Queries::Sortings {

class PrioritySort : public Sort {

  public:
    ~PrioritySort() override = default;

    explicit PrioritySort(bool isAscending);

    PrioritySort(const PrioritySort &) = default;
    PrioritySort(PrioritySort &&) = default;
    auto operator=(const PrioritySort &) -> PrioritySort & = default;
    auto operator=(PrioritySort &&) -> PrioritySort & = default;

    [[nodiscard]] auto clone() const -> std::unique_ptr<Sort> override;

    [[nodiscard]] auto apply(std::vector<const Medium *> media) const
        -> std::vector<const Medium *> override;

  private:
    static auto underlying(MediumUserData::PriorityLevel priority)
        -> std::underlying_type_t<MediumUserData::PriorityLevel>;
};

}

#endif