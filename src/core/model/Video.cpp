#include "Video.h"

#include <QUrlQuery>

namespace Core::Model {

Video::Video(QString&& title, const QUuid id, const QDate dateAdded)
    : Medium{std::move(title), id, dateAdded} {}

std::optional<Video> Video::create(QString title, const QUuid id, const QDate dateAdded) {
    if (!createValidator(title, id, dateAdded)) {
        return std::nullopt;
    }
    return std::make_optional(Video{std::move(title), id, dateAdded});
}

ValidatedField<QUrl>& Video::videoUrl() {
    return videoUrl_;
}
const ValidatedField<QUrl>& Video::videoUrl() const {
    return videoUrl_;
}
bool Video::videoUrlValidator(const QUrl& urlToValidate) {
    if (!urlToValidate.isValid() || urlToValidate.scheme() != "https") {
        return false;
    }

    const QString host{urlToValidate.host()};
    const QString path{urlToValidate.path()};
    if (host == "www.youtube.com" && path == "/watch") {
        const QUrlQuery query{urlToValidate};
        return query.hasQueryItem("v");
    }
    if (host == "youtu.be" && !path.isEmpty() && path != "/") {
        return true;
    }
    return false;
}

ValidatedField<unsigned>& Video::durationSeconds() {
    return durationSeconds_;
}
const ValidatedField<unsigned>& Video::durationSeconds() const {
    return durationSeconds_;
}
bool Video::durationSecondsValidator(const unsigned int durationSecondsToValidate) {
    return durationSecondsToValidate > 0;
}

ValidatedField<QDate>& Video::uploadDate() {
    return uploadDate_;
}
const ValidatedField<QDate>& Video::uploadDate() const {
    return uploadDate_;
}
bool Video::uploadDateValidator(const QDate& dateToValidate) {
    return dateToValidate.isValid() && dateToValidate.year() >= 2005;
}

ValidatedField<QUrl>& Video::thumbnailUrl() {
    return thumbnailUrl_;
}
const ValidatedField<QUrl>& Video::thumbnailUrl() const {
    return thumbnailUrl_;
}
bool Video::thumbnailUrlValidator(const QUrl& urlToValidate) {
    if (!urlToValidate.isValid()) {
        return false;
    }

    const std::vector<QString> allowedExtensions{".png", ".jpg", ".jpeg"};
    const std::vector<QString> allowedSchemes{"http", "https", "file"};

    return std::ranges::any_of(allowedExtensions,
                               [&](const QString& extension) {
                                   return urlToValidate.path().endsWith(extension) ||
                                          urlToValidate.query().endsWith(extension);
                               }) &&
           std::ranges::any_of(allowedSchemes, [&](const QString& scheme) {
               return urlToValidate.scheme() == scheme;
           });
}

}