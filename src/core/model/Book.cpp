#include "Book.h"

#include <QRegularExpression>

namespace Core::Model {

ValidatedField<QString>& Book::isbn() {
    return isbn_;
}
const ValidatedField<QString>& Book::isbn() const {
    return isbn_;
}
bool Book::isbnValidator(const QString& isbnToValidate) {
    static const QRegularExpression regex(
        R"(^(?:ISBN(?:-1[03])?:?\s*)?(?=[-0-9\s]{13,17}$|[0-9X]{10}$|97[89][-0-9\s]{10,16}$)(?:97[89][- ]?)?[0-9]{1,5}[- ]?[0-9]+[- ]?[0-9]+[- ]?[0-9X]$)",
        QRegularExpression::CaseInsensitiveOption);
    return regex.match(isbnToValidate).hasMatch();
}

ValidatedField<QString>& Book::edition() {
    return edition_;
}
const ValidatedField<QString>& Book::edition() const {
    return edition_;
}
bool Book::editionValidator(const QString& editionToValidate) {
    return !editionToValidate.trimmed().isEmpty();
}

ValidatedField<QString>& Book::publisher() {
    return publisher_;
}
const ValidatedField<QString>& Book::publisher() const {
    return publisher_;
}
bool Book::publisherValidator(const QString& publisherToValidate) {
    return !publisherToValidate.trimmed().isEmpty();
}

OptionalField<int>& Book::yearPublished() {
    return yearPublished_;
}
const OptionalField<int>& Book::yearPublished() const {
    return yearPublished_;
}

OptionalField<unsigned int>& Book::pageNumber() {
    return pageNumber_;
}
const OptionalField<unsigned int>& Book::pageNumber() const {
    return pageNumber_;
}

ValidatedField<QString>& Book::description() {
    return description_;
}
const ValidatedField<QString>& Book::description() const {
    return description_;
}
bool Book::descriptionValidator(const QString& descriptionToValidate) {
    return !descriptionToValidate.trimmed().isEmpty();
}

ValidatedField<QUrl>& Book::thumbnailUrl() {
    return thumbnailUrl_;
}
const ValidatedField<QUrl>& Book::thumbnailUrl() const {
    return thumbnailUrl_;
}
bool Book::thumbnailUrlValidator(const QUrl& url) {
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