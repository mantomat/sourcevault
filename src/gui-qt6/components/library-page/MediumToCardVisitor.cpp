#include "MediumToCardVisitor.h"

#include "components/library-page/media-list/LibraryMediumCard.h"

namespace Gui::Components {

auto MediumToCardVisitor::getCardData() const -> LibraryMediumCard::MediumCardViewModel {
    return cardData;
}

void MediumToCardVisitor::visit(const Book &book) {
    cardData = {.id = book.id(),
                .type = "Book",
                .title = book.title(),
                .authors = joinAuthorsSet(book.authors().get()),
                .thumbnailUrl = book.thumbnailUrl().get(),
                .fallbackThumbnailQrcPath = ":/assets/default-thumbnails/book.png"};
}
void MediumToCardVisitor::visit(const Article &article) {
    cardData = {.id = article.id(),
                .type = "Article",
                .title = article.title(),
                .authors = joinAuthorsSet(article.authors().get()),
                .thumbnailUrl = std::nullopt,
                .fallbackThumbnailQrcPath = ":/assets/default-thumbnails/article.png"};
}
void MediumToCardVisitor::visit(const Video &video) {
    cardData = {.id = video.id(),
                .type = "Video",
                .title = video.title(),
                .authors = joinAuthorsSet(video.authors().get()),
                .thumbnailUrl = video.thumbnailUrl().get(),
                .fallbackThumbnailQrcPath = ":/assets/default-thumbnails/video.png"};
}

auto MediumToCardVisitor::joinAuthorsSet(const std::optional<std::set<QString>> &authors)
    -> std::optional<QString> {
    std::optional<QString> authorsOpt{std::nullopt};

    if (authors.has_value()) {
        const std::set<QString> &authorsSet{authors.value()};
        QStringList authorsList{authorsSet.begin(), authorsSet.end()};
        authorsOpt = authorsList.join(", ");
    }

    return authorsOpt;
}
}