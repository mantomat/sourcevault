#include "MediumThumbnailSetter.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;

namespace Core::Persistence::Thumbnails {

void MediumThumbnailSetter::setThumbnailUrl(QUrl newThumbnailUrl) {
    thumbnailUrlToSet = std::move(newThumbnailUrl);
}

auto MediumThumbnailSetter::wasSuccessful() const -> bool {
    return wasSet;
}

void MediumThumbnailSetter::visit(Article & /*unused*/) {
    wasSet = false;
    thumbnailUrlToSet.clear();
}

void MediumThumbnailSetter::visit(Book &book) {
    wasSet = book.thumbnailUrl().set(std::move(thumbnailUrlToSet));
    thumbnailUrlToSet.clear();
}

void MediumThumbnailSetter::visit(Video &video) {
    wasSet = video.thumbnailUrl().set(std::move(thumbnailUrlToSet));
    thumbnailUrlToSet.clear();
}

}