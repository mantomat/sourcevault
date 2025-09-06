#include "MediumThumbnailVisitor.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"

#include <QFileInfo>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;

namespace Core::Persistence::Thumbnails {

auto MediumThumbnailVisitor::getLocalThumbnailPath() const -> std::optional<QString> {
    if (!localThumbnail.has_value() || !localThumbnail->isLocalFile()) {
        return std::nullopt;
    }

    QString path{localThumbnail->toLocalFile()};
    const QFileInfo fileInfo{path};
    if (!fileInfo.exists() || !fileInfo.isReadable()) {
        return std::nullopt;
    }

    return path;
}

void MediumThumbnailVisitor::visit(const Article & /*unused*/) {
    localThumbnail = std::nullopt;
}

void MediumThumbnailVisitor::visit(const Book &book) {
    if (!book.thumbnailUrl().has()) {
        localThumbnail = std::nullopt;
        return;
    }
    localThumbnail = book.thumbnailUrl().get();
}

void MediumThumbnailVisitor::visit(const Video &video) {
    if (!video.thumbnailUrl().has()) {
        localThumbnail = std::nullopt;
        return;
    }
    localThumbnail = video.thumbnailUrl().get();
}

}