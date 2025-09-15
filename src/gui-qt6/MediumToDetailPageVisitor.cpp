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
#include "model/Book.h"
#include "model/Medium.h"
#include "model/MediumUserData.h"
#include "model/Video.h"

using Core::Model::Book;
using Core::Model::Medium;
using Core::Model::MediumUserData;
using Core::Model::Video;
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

MediumToDetailPageVisitor::MediumToDetailPageVisitor(QWidget *newWindowParent,
                                                     QObject *newControllerParent)
    : windowParent{newWindowParent}
    , controllerParent{newControllerParent}
    , mediumDeps{MediumDetailSection::Dependencies{
          .titleValidator = &Medium::titleValidator,
          .authorValidator = &Medium::authorValidator,
          .languageValidator = &Medium::languageValidator,
      }}
    , userDataDeps{UserDataDetailSection::Dependencies{
          .notesValidator = &MediumUserData::notesValidator,
          .topicValidator = &MediumUserData::topicValidator,
      }} {}

auto MediumToDetailPageVisitor::getDetail() const
    -> std::pair<DetailPage *, DetailPageController *> {
    return std::make_pair(detailPage, detailPageController);
}

void MediumToDetailPageVisitor::visit(const Article &article) {
    detailPage =
        new ArticleDetailPage{mediumDeps, userDataDeps,
                              ArticleDetailSection::Dependencies{
                                  .articleUrlValidator = &Article::articleUrlValidator,
                                  .sourceNameValidator = &Article::sourceNameValidator,
                                  .readTimeMinutesValidator = &Article::readTimeMinutesValidator,
                                  .publicationDateValidator = &Article::publicationDateValidator},
                              windowParent};

    detailPageController = new ArticleDetailPageController{
        dynamic_cast<ArticleDetailPage *>(detailPage), &article, controllerParent};
}
void MediumToDetailPageVisitor::visit(const Book &book) {
    detailPage = new BookDetailPage{
        mediumDeps, userDataDeps,
        BookDetailSection::Dependencies{.isbnValidator = &Book::isbnValidator,
                                        .editionValidator = &Book::editionValidator,
                                        .publisherValidator = &Book::publisherValidator,
                                        .yearPublishedValidator = &Book::yearPublishedValidator,
                                        .pageNumberValidator = &Book::pageNumberValidator,
                                        .descriptionValidator = &Book::descriptionValidator,
                                        .thumbnailUrlValidator = &Book::thumbnailUrlValidator},
        windowParent};

    detailPageController = new BookDetailPageController{dynamic_cast<BookDetailPage *>(detailPage),
                                                        &book, controllerParent};
}
void MediumToDetailPageVisitor::visit(const Video &video) {
    detailPage =
        new VideoDetailPage{mediumDeps, userDataDeps,
                            VideoDetailSection::Dependencies{
                                .videoUrlValidator = &Video::videoUrlValidator,
                                .durationSecondsValidator = &Video::durationSecondsValidator,
                                .uploadDateValidator = &Video::uploadDateValidator,
                                .thumbnailUrlValidator = &Video::thumbnailUrlValidator},
                            windowParent};

    detailPageController = new VideoDetailPageController{
        dynamic_cast<VideoDetailPage *>(detailPage), &video, controllerParent};
}

}