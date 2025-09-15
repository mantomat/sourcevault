#ifndef MEDIUMTODETAILPAGEVISITOR_H
#define MEDIUMTODETAILPAGEVISITOR_H

#include "DialogsController.h"
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

#include <QObject>

namespace Gui {

class MediumToDetailPageVisitor : public QObject, public MediumVisitor {
    Q_OBJECT

  private:
    DialogsController *dialogsController;
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
    ~MediumToDetailPageVisitor() override = default;

    MediumToDetailPageVisitor(MediumDetailSection::Dependencies newMediumDeps,
                              UserDataDetailSection::Dependencies newUserDataDeps,
                              VideoDetailSection::Dependencies newVideoDeps,
                              BookDetailSection::Dependencies newBookDeps,
                              ArticleDetailSection::Dependencies newArticleDeps,
                              DialogsController *newDialogsController, QWidget *newWindowParent,
                              QObject *newControllerParent);

    MediumToDetailPageVisitor(const MediumToDetailPageVisitor &) = delete;
    MediumToDetailPageVisitor(MediumToDetailPageVisitor &&) = delete;
    auto operator=(const MediumToDetailPageVisitor &) -> MediumToDetailPageVisitor & = delete;
    auto operator=(MediumToDetailPageVisitor &&) -> MediumToDetailPageVisitor & = delete;

    [[nodiscard]] auto getDetail() const -> std::pair<DetailPage *, DetailPageController *>;

    void visit(const Article &article) override;
    void visit(const Book &book) override;
    void visit(const Video &video) override;
};

}

#endif