#ifndef MEDIUMTYPEVISITOR_H
#define MEDIUMTYPEVISITOR_H

#include "MediumVisitor.h"

#include <typeindex>
#include <unordered_set>

namespace Core::Shared {

class MediumTypeVisitor : public MediumVisitor {
    bool match{false};
    std::unordered_set<std::type_index> targetTypes;

  public:
    ~MediumTypeVisitor() override = default;
    explicit MediumTypeVisitor(std::unordered_set<std::type_index> newTargetTypes);
    MediumTypeVisitor(const MediumTypeVisitor &) = default;
    MediumTypeVisitor(MediumTypeVisitor &&) = default;
    auto operator=(const MediumTypeVisitor &) -> MediumTypeVisitor & = default;
    auto operator=(MediumTypeVisitor &&) -> MediumTypeVisitor & = default;

    [[nodiscard]] auto hasMatched() const -> bool;

    void visit(const Book &book) override;
    void visit(const Article &article) override;
    void visit(const Video &video) override;
};

}

#endif