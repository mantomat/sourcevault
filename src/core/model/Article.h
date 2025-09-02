#ifndef ARTICLE_H
#define ARTICLE_H

#include "Medium.h"

#include <QDate>
#include <QUrl>

namespace Core::Model {

class Article : public Medium {

    ValidatedField<QUrl> articleUrl_{articleUrlValidator};
    ValidatedField<QString> sourceName_{sourceNameValidator};
    ValidatedField<unsigned int> readTimeMinutes_{readTimeMinutesValidator};
    ValidatedField<QDate> publicationDate_{publicationDateValidator};

    /**
     * @brief Constructs a new Article.
     *
     * QUuid and QDate are passed by value since they are trivially copyable.
     */
    Article(QString &&title, QUuid id, QDate dateAdded);

  public:
    ~Article() override = default;
    Article(const Article &) = default;
    Article(Article &&) = default;
    auto operator=(const Article &) -> Article & = default;
    auto operator=(Article &&) -> Article & = default;

    [[nodiscard]] auto operator==(const Article &) const -> bool = default;

    [[nodiscard]] auto clone() const -> std::unique_ptr<Medium> override;

    [[nodiscard]] static auto create(QString title, QUuid id = QUuid::createUuid(),
                                     QDate dateAdded = QDate::currentDate())
        -> std::optional<Article>;
    [[nodiscard]] static auto make(QString title, QUuid id = QUuid::createUuid(),
                                   QDate dateAdded = QDate::currentDate())
        -> std::optional<std::unique_ptr<Article>>;

    [[nodiscard]] auto articleUrl() -> ValidatedField<QUrl> &;
    [[nodiscard]] auto articleUrl() const -> const ValidatedField<QUrl> &;
    [[nodiscard]] static auto articleUrlValidator(const QUrl &urlToValidate) -> bool;

    [[nodiscard]] auto sourceName() -> ValidatedField<QString> &;
    [[nodiscard]] auto sourceName() const -> const ValidatedField<QString> &;
    [[nodiscard]] static auto sourceNameValidator(const QString &sourceNameToValidate) -> bool;

    [[nodiscard]] auto readTimeMinutes() -> ValidatedField<unsigned int> &;
    [[nodiscard]] auto readTimeMinutes() const -> const ValidatedField<unsigned int> &;
    [[nodiscard]] static auto readTimeMinutesValidator(unsigned int readTimeMinutesToValidate)
        -> bool;

    [[nodiscard]] auto publicationDate() -> ValidatedField<QDate> &;
    [[nodiscard]] auto publicationDate() const -> const ValidatedField<QDate> &;
    [[nodiscard]] static auto publicationDateValidator(const QDate &dateToValidate) -> bool;

    void accept(MediumVisitor &visitor) const override;
};

}

#endif
