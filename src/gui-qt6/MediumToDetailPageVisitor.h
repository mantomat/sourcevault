#ifndef MEDIUMTODETAILPAGEVISITOR_H
#define MEDIUMTODETAILPAGEVISITOR_H

#include "components/medium-detail/DetailPage.h"
#include "components/medium-detail/DetailPageController.h"
#include "components/medium-detail/detail-sections/MediumDetailSection.h"
#include "components/medium-detail/detail-sections/UserDataDetailSection.h"
#include "shared/MediumVisitor.h"

using Core::Shared::MediumVisitor;
using Gui::Components::DetailPage;
using Gui::Components::DetailPageController;
using Gui::Components::MediumDetailSection;
using Gui::Components::UserDataDetailSection;

namespace Gui {

class MediumToDetailPageVisitor : public MediumVisitor {

  private:
    QWidget *windowParent;
    QObject *controllerParent;

    MediumDetailSection::Dependencies mediumDeps;
    UserDataDetailSection::Dependencies userDataDeps;

    DetailPage *detailPage{nullptr};
    DetailPageController *detailPageController{nullptr};

  public:
    ~MediumToDetailPageVisitor() = default;

    MediumToDetailPageVisitor(QWidget *windowParent, QObject *controllerParent);

    MediumToDetailPageVisitor(const MediumToDetailPageVisitor &) = default;
    MediumToDetailPageVisitor(MediumToDetailPageVisitor &&) = default;
    auto operator=(const MediumToDetailPageVisitor &) -> MediumToDetailPageVisitor & = default;
    auto operator=(MediumToDetailPageVisitor &&) -> MediumToDetailPageVisitor & = default;

    [[nodiscard]] auto getDetail() const -> std::pair<DetailPage *, DetailPageController *>;

    void visit(const Article &article) override;
    void visit(const Book &book) override;
    void visit(const Video &video) override;
};

}

#endif