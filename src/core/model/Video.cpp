#include "Video.h"

#include <QUrlQuery>

namespace Core::Model {

Video::Video(QString&& title, const QUuid id, const QDate dateAdded)
    : Medium{std::move(title), id, dateAdded} {}

auto Video::clone() const -> std::unique_ptr<Medium> {
    return std::make_unique<Video>(*this);
}

auto Video::create(QString title, const QUuid id, const QDate dateAdded) -> std::optional<Video> {
    if (!createValidator(title, id, dateAdded)) {
        return std::nullopt;
    }
    return std::make_optional(Video{std::move(title), id, dateAdded});
}

auto Video::videoUrl() -> ValidatedField<QUrl>& {
    return videoUrl_;
}
auto Video::videoUrl() const -> const ValidatedField<QUrl>& {
    return videoUrl_;
}
auto Video::videoUrlValidator(const QUrl& urlToValidate) -> bool {
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

auto Video::durationSeconds() -> ValidatedField<unsigned>& {
    return durationSeconds_;
}
auto Video::durationSeconds() const -> const ValidatedField<unsigned>& {
    return durationSeconds_;
}
auto Video::durationSecondsValidator(const unsigned int durationSecondsToValidate) -> bool {
    return durationSecondsToValidate > 0;
}

auto Video::uploadDate() -> ValidatedField<QDate>& {
    return uploadDate_;
}
auto Video::uploadDate() const -> const ValidatedField<QDate>& {
    return uploadDate_;
}
constexpr int minUploadDateYear{2005};
auto Video::uploadDateValidator(const QDate& dateToValidate) -> bool {
    return dateToValidate.isValid() && dateToValidate.year() >= minUploadDateYear;
}

auto Video::thumbnailUrl() -> ValidatedField<QUrl>& {
    return thumbnailUrl_;
}
auto Video::thumbnailUrl() const -> const ValidatedField<QUrl>& {
    return thumbnailUrl_;
}
auto Video::thumbnailUrlValidator(const QUrl& urlToValidate) -> bool {
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