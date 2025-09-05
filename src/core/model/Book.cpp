#include "Book.h"

#include <QRegularExpression>
#include <algorithm>

namespace Core::Model {

Book::Book(QString &&title, const QUuid id, const QDate dateAdded)
    : Medium{std::move(title), id, dateAdded} {}

auto Book::clone() const -> std::unique_ptr<Medium> {
    return std::make_unique<Book>(*this);
}

auto Book::create(QString title, const QUuid id, const QDate dateAdded) -> std::optional<Book> {
    if (!createValidator(title, id, dateAdded)) {
        return std::nullopt;
    }
    return std::make_optional(Book{std::move(title), id, dateAdded});
}

auto Book::make(QString title, const QUuid id, const QDate dateAdded)
    -> std::optional<std::unique_ptr<Book>> {
    auto bookOpt{Book::create(std::move(title), id, dateAdded)};
    if (!bookOpt.has_value()) {
        return std::nullopt;
    }
    return std::make_unique<Book>(std::move(bookOpt.value()));
}

auto Book::isbn() -> ValidatedField<QString> & {
    return isbn_;
}
auto Book::isbn() const -> const ValidatedField<QString> & {
    return isbn_;
}
auto Book::isbnValidator(const QString &isbnToValidate) -> bool {
    static const QRegularExpression regex(
        R"(^(?:ISBN(?:-1[03])?:?\s*)?(?=[-0-9\s]{13,17}$|[0-9X]{10}$|97[89][-0-9\s]{10,16}$)(?:97[89][- ]?)?[0-9]{1,5}[- ]?[0-9]+[- ]?[0-9]+[- ]?[0-9X]$)",
        QRegularExpression::CaseInsensitiveOption);
    return regex.match(isbnToValidate).hasMatch();
}

auto Book::edition() -> ValidatedField<QString> & {
    return edition_;
}
auto Book::edition() const -> const ValidatedField<QString> & {
    return edition_;
}
auto Book::editionValidator(const QString &editionToValidate) -> bool {
    return !editionToValidate.trimmed().isEmpty();
}

auto Book::publisher() -> ValidatedField<QString> & {
    return publisher_;
}
auto Book::publisher() const -> const ValidatedField<QString> & {
    return publisher_;
}
auto Book::publisherValidator(const QString &publisherToValidate) -> bool {
    return !publisherToValidate.trimmed().isEmpty();
}

auto Book::yearPublished() -> ValidatedField<int> & {
    return yearPublished_;
}
auto Book::yearPublished() const -> const ValidatedField<int> & {
    return yearPublished_;
}
constexpr int minPublishingYear{-10000};
auto Book::yearPublishedValidator(const int yearPublishedToValidate) -> bool {
    return yearPublishedToValidate >= minPublishingYear;
}

auto Book::pageNumber() -> ValidatedField<unsigned int> & {
    return pageNumber_;
}
auto Book::pageNumber() const -> const ValidatedField<unsigned int> & {
    return pageNumber_;
}
auto Book::pageNumberValidator(const unsigned int pageNumberToValidate) -> bool {
    return pageNumberToValidate > 0;
}

auto Book::description() -> ValidatedField<QString> & {
    return description_;
}
auto Book::description() const -> const ValidatedField<QString> & {
    return description_;
}
auto Book::descriptionValidator(const QString &descriptionToValidate) -> bool {
    return !descriptionToValidate.trimmed().isEmpty();
}

auto Book::thumbnailUrl() -> ValidatedField<QUrl> & {
    return thumbnailUrl_;
}
auto Book::thumbnailUrl() const -> const ValidatedField<QUrl> & {
    return thumbnailUrl_;
}
auto Book::thumbnailUrlValidator(const QUrl &urlToValidate) -> bool {
    if (!urlToValidate.isValid()) {
        return false;
    }

    const std::vector<QString> allowedExtensions{".png", ".jpg", ".jpeg"};
    const std::vector<QString> allowedSchemes{"http", "https", "file"};

    return std::ranges::any_of(allowedExtensions,
                               [&](const QString &extension) {
                                   return urlToValidate.path().endsWith(extension) ||
                                          urlToValidate.query().endsWith(extension);
                               }) &&
           std::ranges::any_of(allowedSchemes, [&](const QString &scheme) {
               return urlToValidate.scheme() == scheme;
           });
}

void Book::accept(MediumVisitor &visitor) const {
    visitor.visit(*this);
}
void Book::accept(MutMediumVisitor &visitor) {
    visitor.visit(*this);
}

}