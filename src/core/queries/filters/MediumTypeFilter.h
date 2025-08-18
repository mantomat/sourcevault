#ifndef MEDIUMTYPEFILTER_H
#define MEDIUMTYPEFILTER_H

#include "queries/filters/Filter.h"
#include "shared/MediumTypeVisitor.h"

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;

using Core::Shared::MediumTypeVisitor;

namespace Core::Queries::Filters {

class MediumTypeFilter final : public Filter {

    std::unique_ptr<MediumTypeVisitor> visitor;

    void swap(MediumTypeFilter &other) noexcept;

  public:
    ~MediumTypeFilter() override = default;

    explicit MediumTypeFilter(std::unordered_set<std::type_index> types);

    MediumTypeFilter(const MediumTypeFilter &);
    MediumTypeFilter(MediumTypeFilter &&) noexcept = default;
    auto operator=(const MediumTypeFilter &) -> MediumTypeFilter &;
    auto operator=(MediumTypeFilter &&) noexcept -> MediumTypeFilter & = default;

    [[nodiscard]] auto clone() const -> std::unique_ptr<Filter> override;
    [[nodiscard]] auto matches(const Medium *medium) const -> bool override;
};

}

#endif