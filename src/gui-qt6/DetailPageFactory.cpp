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

auto DetailPageFactory::createDetailPage(const Core::Model::Medium *medium, QWidget *windowParent,
                                         QObject *controllerParent)
    -> std::pair<DetailPage *, DetailPageController *> {
    MediumToDetailPageVisitor visitor{mediumDeps,  userDataDeps, videoDeps,       bookDeps,
                                      articleDeps, windowParent, controllerParent};

    medium->accept(visitor);
    return visitor.getDetail();
}

auto DetailPageFactory::createNewMediumPage(LibraryTopbar::MediumTypeViewModel mediumType,
                                            QWidget *windowParent, QObject *controllerParent)
    -> std::pair<DetailPage *, CreateController *> {

    switch (mediumType) {
    case Components::LibraryTopbar::MediumTypeViewModel::Article: {
        auto *articleDetailPage{
            new ArticleDetailPage{mediumDeps, userDataDeps, articleDeps, windowParent}};
        auto *articleController{new ArticleCreateController{articleDetailPage, controllerParent}};
        return std::make_pair(articleDetailPage, articleController);
    }
    case Components::LibraryTopbar::MediumTypeViewModel::Book: {
        auto *bookDetailPage{new BookDetailPage{mediumDeps, userDataDeps, bookDeps, windowParent}};
        auto *bookController{new BookCreateController{bookDetailPage, controllerParent}};
        return std::make_pair(bookDetailPage, bookController);
    }
    case Components::LibraryTopbar::MediumTypeViewModel::Video: {
        auto *videoDetailPage{
            new VideoDetailPage{mediumDeps, userDataDeps, videoDeps, windowParent}};
        auto *videoController{new VideoCreateController{videoDetailPage, controllerParent}};
        return std::make_pair(videoDetailPage, videoController);
    }
    }

    assert(false);
    return std::make_pair(nullptr, nullptr);
}
}