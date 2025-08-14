#include "Book.h"

#include <QRegularExpression>
#include <algorithm>

namespace Core::Model {

Book::Book(QString&& title, const QUuid id, const QDate dateAdded)
    : Medium{std::move(title), id, dateAdded} {}

std::unique_ptr<Medium> Book::clone() const {
    return std::make_unique<Book>(*this);
}

std::optional<Book> Book::create(QString title, const QUuid id, const QDate dateAdded) {
    if (!createValidator(title, id, dateAdded)) {
        return std::nullopt;
    }
    return std::make_optional(Book{std::move(title), id, dateAdded});
}

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

ValidatedField<int>& Book::yearPublished() {
    return yearPublished_;
}
const ValidatedField<int>& Book::yearPublished() const {
    return yearPublished_;
}
bool Book::yearPublishedValidator(const int yearPublishedToValidate) {
    return yearPublishedToValidate >= -10000;
}

ValidatedField<unsigned int>& Book::pageNumber() {
    return pageNumber_;
}
const ValidatedField<unsigned int>& Book::pageNumber() const {
    return pageNumber_;
}
bool Book::pageNumberValidator(const unsigned int pageNumberToValidate) {
    return pageNumberToValidate > 0;
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
bool Book::thumbnailUrlValidator(const QUrl& urlToValidate) {
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