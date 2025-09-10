#ifndef MEDIUMTOCARDVISITOR_H
#define MEDIUMTOCARDVISITOR_H

#include "media-list/LibraryMediumCard.h"
#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "shared/MediumVisitor.h"

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;
using Core::Shared::MediumVisitor;

namespace Gui::Components {

class MediumToCardVisitor : public MediumVisitor {

    LibraryMediumCard::MediumCardViewModel cardData;

  public:
    ~MediumToCardVisitor() override = default;
    MediumToCardVisitor() = default;
    MediumToCardVisitor(const MediumToCardVisitor &) = default;
    MediumToCardVisitor(MediumToCardVisitor &&) = default;
    auto operator=(const MediumToCardVisitor &) -> MediumToCardVisitor & = default;
    auto operator=(MediumToCardVisitor &&) -> MediumToCardVisitor & = default;

    [[nodiscard]] auto getCardData() const -> LibraryMediumCard::MediumCardViewModel;

    void visit(const Book &book) override;
    void visit(const Article &article) override;
    void visit(const Video &video) override;

  private:
    static auto joinAuthorsSet(const std::optional<std::set<QString>> &authors)
        -> std::optional<QString>;
};

}

#endif