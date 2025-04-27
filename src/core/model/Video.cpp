#include "Video.h"

#include <QUrlQuery>

namespace Core::Model {

ValidatedField<QUrl>& Video::videoUrl() {
    return videoUrl_;
}
const ValidatedField<QUrl>& Video::videoUrl() const {
    return videoUrl_;
}
bool Video::videoUrlValidator(const QUrl& url) {
    if (!url.isValid()) {
        return false;
    }

    const QString host{url.host()};
    const QString path{url.path()};
    if (host == "www.youtube.com" && path == "/watch") {
        const QUrlQuery query{url};
        return query.hasQueryItem("v");
    }
    if (host == "youtu.be" && !path.isEmpty() && path != "/") {
        return true;
    }
    return false;
}

OptionalField<unsigned>& Video::durationSeconds() {
    return durationSeconds_;
}
const OptionalField<unsigned>& Video::durationSeconds() const {
    return durationSeconds_;
}

ValidatedField<QDate>& Video::uploadDate() {
    return uploadDate_;
}
const ValidatedField<QDate>& Video::uploadDate() const {
    return uploadDate_;
}
bool Video::uploadDateValidator(const QDate& date) {
    return date.isValid();
}

ValidatedField<QUrl>& Video::thumbnailUrl() {
    return thumbnailUrl_;
}
const ValidatedField<QUrl>& Video::thumbnailUrl() const {
    return thumbnailUrl_;
}
bool Video::thumbnailUrlValidator(const QUrl& url) {
    if (!url.isValid()) {
        return false;
    }

    const std::vector<QString> allowedExtensions{".png", ".jpg", ".jpeg"};

    const QString path{url.path()};
    const QString query{url.query()};

    return std::ranges::any_of(allowedExtensions, [&](const QString& extension) {
        return path.endsWith(extension) || query.endsWith(extension);
    });
}
}