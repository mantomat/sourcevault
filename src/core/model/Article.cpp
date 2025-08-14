#include "Article.h"

namespace Core::Model {

Article::Article(QString&& title, const QUuid id, const QDate dateAdded)
    : Medium{std::move(title), id, dateAdded} {}

std::optional<Article> Article::create(QString title, const QUuid id, const QDate dateAdded) {
    if (!createValidator(title, id, dateAdded)) {
        return std::nullopt;
    }
    return std::make_optional(Article{std::move(title), id, dateAdded});
}

ValidatedField<QUrl>& Article::articleUrl() {
    return articleUrl_;
}
const ValidatedField<QUrl>& Article::articleUrl() const {
    return articleUrl_;
}
bool Article::articleUrlValidator(const QUrl& urlToValidate) {
    const std::set<QString> allowedSchemes{"http", "https"};
    return urlToValidate.isValid() && allowedSchemes.contains(urlToValidate.scheme());
}

ValidatedField<QString>& Article::sourceName() {
    return sourceName_;
}
const ValidatedField<QString>& Article::sourceName() const {
    return sourceName_;
}
bool Article::sourceNameValidator(const QString& sourceNameToValidate) {
    return !sourceNameToValidate.trimmed().isEmpty();
}

ValidatedField<unsigned int>& Article::readTimeMinutes() {
    return readTimeMinutes_;
}
const ValidatedField<unsigned int>& Article::readTimeMinutes() const {
    return readTimeMinutes_;
}
bool Article::readTimeMinutesValidator(const unsigned int readTimeMinutesToValidate) {
    return readTimeMinutesToValidate > 0;
}

ValidatedField<QDate>& Article::publicationDate() {
    return publicationDate_;
}
const ValidatedField<QDate>& Article::publicationDate() const {
    return publicationDate_;
}
bool Article::publicationDateValidator(const QDate& dateToValidate) {
    return dateToValidate.isValid();
}

}