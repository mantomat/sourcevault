#include "MediumToDetailPageVisitor.h"

#include "components/medium-detail/ArticleDetailPage.h"
#include "components/medium-detail/ArticleDetailPageController.h"
#include "components/medium-detail/BookDetailPage.h"
#include "components/medium-detail/BookDetailPageController.h"
#include "components/medium-detail/VideoDetailPage.h"
#include "components/medium-detail/VideoDetailPageController.h"
#include "components/medium-detail/detail-sections/ArticleDetailSection.h"
#include "components/medium-detail/detail-sections/BookDetailSection.h"
#include "components/medium-detail/detail-sections/VideoDetailSection.h"

using Gui::Components::ArticleDetailPage;
using Gui::Components::ArticleDetailPageController;
using Gui::Components::ArticleDetailSection;
using Gui::Components::BookDetailPage;
using Gui::Components::BookDetailPageController;
using Gui::Components::BookDetailSection;
using Gui::Components::VideoDetailPage;
using Gui::Components::VideoDetailPageController;
using Gui::Components::VideoDetailSection;

namespace Gui {

MediumToDetailPageVisitor::MediumToDetailPageVisitor(
    MediumDetailSection::Dependencies newMediumDeps,
    UserDataDetailSection::Dependencies newUserDataDeps,
    VideoDetailSection::Dependencies newVideoDeps, BookDetailSection::Dependencies newBookDeps,
    ArticleDetailSection::Dependencies newArticleDeps, DialogsController *newDialogsController,
    QWidget *newWindowParent, QObject *newControllerParent)
    : QObject{newControllerParent}
    , dialogsController{newDialogsController}
    , windowParent{newWindowParent}
    , controllerParent{newControllerParent}
    , mediumDeps{std::move(newMediumDeps)}
    , userDataDeps{std::move(newUserDataDeps)}
    , videoDeps{std::move(newVideoDeps)}
    , bookDeps{std::move(newBookDeps)}
    , articleDeps{std::move(newArticleDeps)} {}

auto MediumToDetailPageVisitor::getDetail() const
    -> std::pair<DetailPage *, DetailPageController *> {
    return std::make_pair(detailPage, detailPageController);
}

void MediumToDetailPageVisitor::visit(const Article &article) {
    detailPage = new ArticleDetailPage{mediumDeps, userDataDeps, articleDeps, windowParent};

    detailPageController = new ArticleDetailPageController{
        dynamic_cast<ArticleDetailPage *>(detailPage), &article, controllerParent};
}
void MediumToDetailPageVisitor::visit(const Book &book) {
    detailPage = new BookDetailPage{mediumDeps, userDataDeps, bookDeps, windowParent};

    detailPageController = new BookDetailPageController{dynamic_cast<BookDetailPage *>(detailPage),
                                                        &book, controllerParent};

    connect(dynamic_cast<BookDetailPage *>(detailPage)->getBookSection(),
            &BookDetailSection::thumbnailPathDialogRequested, dialogsController,
            &DialogsController::onRequestThumbnailUrlDialog);
    connect(dialogsController, &DialogsController::thumbnailUrlPathChosen,
            dynamic_cast<BookDetailPage *>(detailPage)->getBookSection(),
            &BookDetailSection::onThumbnailPathSelected);
}
void MediumToDetailPageVisitor::visit(const Video &video) {
    detailPage = new VideoDetailPage{mediumDeps, userDataDeps, videoDeps, windowParent};

    detailPageController = new VideoDetailPageController{
        dynamic_cast<VideoDetailPage *>(detailPage), &video, controllerParent};

    connect(dynamic_cast<VideoDetailPage *>(detailPage)->getVideoSection(),
            &VideoDetailSection::thumbnailPathDialogRequested, dialogsController,
            &DialogsController::onRequestThumbnailUrlDialog);
    connect(dialogsController, &DialogsController::thumbnailUrlPathChosen,
            dynamic_cast<VideoDetailPage *>(detailPage)->getVideoSection(),
            &VideoDetailSection::onThumbnailPathSelected);
}
}