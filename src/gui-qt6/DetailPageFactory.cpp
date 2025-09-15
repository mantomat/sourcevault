#include "DetailPageFactory.h"

#include "MediumToDetailPageVisitor.h"
#include "components/medium-detail/ArticleCreateController.h"
#include "components/medium-detail/ArticleDetailPage.h"
#include "components/medium-detail/BookCreateController.h"
#include "components/medium-detail/BookDetailPage.h"
#include "components/medium-detail/VideoCreateController.h"
#include "components/medium-detail/VideoDetailPage.h"

using Gui::Components::ArticleCreateController;
using Gui::Components::ArticleDetailPage;
using Gui::Components::BookCreateController;
using Gui::Components::BookDetailPage;
using Gui::Components::VideoCreateController;
using Gui::Components::VideoDetailPage;

namespace Gui {

const MediumDetailSection::Dependencies DetailPageFactory::mediumDeps{
    .titleValidator = &Medium::titleValidator,
    .authorValidator = &Medium::authorValidator,
    .languageValidator = &Medium::languageValidator,
};

const UserDataDetailSection::Dependencies DetailPageFactory::userDataDeps{
    .notesValidator = &MediumUserData::notesValidator,
    .topicValidator = &MediumUserData::topicValidator,
};

const ArticleDetailSection::Dependencies DetailPageFactory::articleDeps{
    .articleUrlValidator = &Article::articleUrlValidator,
    .sourceNameValidator = &Article::sourceNameValidator,
    .readTimeMinutesValidator = &Article::readTimeMinutesValidator,
    .publicationDateValidator = &Article::publicationDateValidator,
};

const BookDetailSection::Dependencies DetailPageFactory::bookDeps{
    .isbnValidator = &Book::isbnValidator,
    .editionValidator = &Book::editionValidator,
    .publisherValidator = &Book::publisherValidator,
    .yearPublishedValidator = &Book::yearPublishedValidator,
    .pageNumberValidator = &Book::pageNumberValidator,
    .descriptionValidator = &Book::descriptionValidator,
    .thumbnailUrlValidator = &Book::thumbnailUrlValidator,
};
const VideoDetailSection::Dependencies DetailPageFactory::videoDeps{
    .videoUrlValidator = &Video::videoUrlValidator,
    .durationSecondsValidator = &Video::durationSecondsValidator,
    .uploadDateValidator = &Video::uploadDateValidator,
    .thumbnailUrlValidator = &Video::thumbnailUrlValidator,
};

auto DetailPageFactory::createDetailPage(const Core::Model::Medium *medium,
                                         DialogsController *dialogsController,
                                         QWidget *windowParent, QObject *controllerParent)
    -> std::pair<DetailPage *, DetailPageController *> {
    MediumToDetailPageVisitor visitor{mediumDeps,   userDataDeps,    videoDeps,
                                      bookDeps,     articleDeps,     dialogsController,
                                      windowParent, controllerParent};

    medium->accept(visitor);
    return visitor.getDetail();
}

auto DetailPageFactory::createNewMediumPage(LibraryTopbar::MediumTypeViewModel mediumType,
                                            DialogsController *dialogsController,
                                            QWidget *windowParent, QObject *controllerParent)
    -> std::pair<DetailPage *, CreateController *> {

    switch (mediumType) {
    case Components::LibraryTopbar::MediumTypeViewModel::Article: {
        auto *detailPage{
            new ArticleDetailPage{mediumDeps, userDataDeps, articleDeps, windowParent}};
        auto *controller{new ArticleCreateController{detailPage, controllerParent}};

        return std::make_pair(detailPage, controller);
    }
    case Components::LibraryTopbar::MediumTypeViewModel::Book: {
        auto *detailPage{new BookDetailPage{mediumDeps, userDataDeps, bookDeps, windowParent}};
        auto *controller{new BookCreateController{detailPage, controllerParent}};

        connect(detailPage->getBookSection(), &BookDetailSection::thumbnailPathDialogRequested,
                dialogsController, &DialogsController::onRequestThumbnailUrlDialog);
        connect(dialogsController, &DialogsController::thumbnailUrlPathChosen,
                detailPage->getBookSection(), &BookDetailSection::onThumbnailPathSelected);

        return std::make_pair(detailPage, controller);
    }
    case Components::LibraryTopbar::MediumTypeViewModel::Video: {
        auto *detailPage{new VideoDetailPage{mediumDeps, userDataDeps, videoDeps, windowParent}};
        auto *controller{new VideoCreateController{detailPage, controllerParent}};

        connect(detailPage->getVideoSection(), &VideoDetailSection::thumbnailPathDialogRequested,
                dialogsController, &DialogsController::onRequestThumbnailUrlDialog);
        connect(dialogsController, &DialogsController::thumbnailUrlPathChosen,
                detailPage->getVideoSection(), &VideoDetailSection::onThumbnailPathSelected);
        return std::make_pair(detailPage, controller);
    }
    }

    assert(false);
    return std::make_pair(nullptr, nullptr);
}
}