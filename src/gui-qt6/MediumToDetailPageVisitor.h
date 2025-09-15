#ifndef MEDIUMTODETAILPAGEVISITOR_H
#define MEDIUMTODETAILPAGEVISITOR_H

#include "components/medium-detail/DetailPage.h"
#include "components/medium-detail/DetailPageController.h"
#include "components/medium-detail/detail-sections/ArticleDetailSection.h"
#include "components/medium-detail/detail-sections/BookDetailSection.h"
#include "components/medium-detail/detail-sections/MediumDetailSection.h"
#include "components/medium-detail/detail-sections/UserDataDetailSection.h"
#include "components/medium-detail/detail-sections/VideoDetailSection.h"
#include "shared/MediumVisitor.h"

using Core::Shared::MediumVisitor;
using Gui::Components::ArticleDetailSection;
using Gui::Components::BookDetailSection;
using Gui::Components::DetailPage;
using Gui::Components::DetailPageController;
using Gui::Components::MediumDetailSection;
using Gui::Components::UserDataDetailSection;
using Gui::Components::VideoDetailSection;

namespace Gui {

class MediumToDetailPageVisitor : public MediumVisitor {

  private:
    QWidget *windowParent;
    QObject *controllerParent;

    MediumDetailSection::Dependencies mediumDeps;
    UserDataDetailSection::Dependencies userDataDeps;
    VideoDetailSection::Dependencies videoDeps;
    BookDetailSection::Dependencies bookDeps;
    ArticleDetailSection::Dependencies articleDeps;

    DetailPage *detailPage{nullptr};
    DetailPageController *detailPageController{nullptr};

  public:
    ~MediumToDetailPageVisitor() = default;

    MediumToDetailPageVisitor(MediumDetailSection::Dependencies newMediumDeps,
                              UserDataDetailSection::Dependencies newUserDataDeps,
                              VideoDetailSection::Dependencies newVideoDeps,
                              BookDetailSection::Dependencies newBookDeps,
                              ArticleDetailSection::Dependencies newArticleDeps,
                              QWidget *newWindowParent, QObject *newControllerParent);

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