#include "Article.h"

namespace Core::Model {

ValidatedField<QUrl>& Article::articleUrl() {
    return articleUrl_;
}
const ValidatedField<QUrl>& Article::articleUrl() const {
    return articleUrl_;
}
bool Article::articleUrlValidator(const QUrl& urlToValidate) {
    return urlToValidate.isValid();
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

OptionalField<unsigned int>& Article::readTimeMinutes() {
    return readTimeMinutes_;
}
const OptionalField<unsigned int>& Article::readTimeMinutes() const {
    return readTimeMinutes_;
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