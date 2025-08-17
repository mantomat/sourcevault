#include "Article.h"

namespace Core::Model {

Article::Article(QString&& title, const QUuid id, const QDate dateAdded)
    : Medium{std::move(title), id, dateAdded} {}

auto Article::clone() const -> std::unique_ptr<Medium> {
    return std::make_unique<Article>(*this);
}

auto Article::create(QString title, const QUuid id, const QDate dateAdded)
    -> std::optional<Article> {
    if (!createValidator(title, id, dateAdded)) {
        return std::nullopt;
    }
    return std::make_optional(Article{std::move(title), id, dateAdded});
}

auto Article::articleUrl() -> ValidatedField<QUrl>& {
    return articleUrl_;
}
auto Article::articleUrl() const -> const ValidatedField<QUrl>& {
    return articleUrl_;
}
auto Article::articleUrlValidator(const QUrl& urlToValidate) -> bool {
    const std::set<QString> allowedSchemes{"http", "https"};
    return urlToValidate.isValid() && allowedSchemes.contains(urlToValidate.scheme());
}

auto Article::sourceName() -> ValidatedField<QString>& {
    return sourceName_;
}
auto Article::sourceName() const -> const ValidatedField<QString>& {
    return sourceName_;
}
auto Article::sourceNameValidator(const QString& sourceNameToValidate) -> bool {
    return !sourceNameToValidate.trimmed().isEmpty();
}

auto Article::readTimeMinutes() -> ValidatedField<unsigned int>& {
    return readTimeMinutes_;
}
auto Article::readTimeMinutes() const -> const ValidatedField<unsigned int>& {
    return readTimeMinutes_;
}
auto Article::readTimeMinutesValidator(const unsigned int readTimeMinutesToValidate) -> bool {
    return readTimeMinutesToValidate > 0;
}

auto Article::publicationDate() -> ValidatedField<QDate>& {
    return publicationDate_;
}
auto Article::publicationDate() const -> const ValidatedField<QDate>& {
    return publicationDate_;
}
auto Article::publicationDateValidator(const QDate& dateToValidate) -> bool {
    return dateToValidate.isValid();
}

}
