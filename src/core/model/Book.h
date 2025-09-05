#ifndef BOOK_H
#define BOOK_H

#include "Medium.h"

#include <QUrl>
#include <memory>

namespace Core::Model {

class Book : public Medium {

    ValidatedField<QString> isbn_{isbnValidator};
    ValidatedField<QString> edition_{editionValidator};
    ValidatedField<QString> publisher_{publisherValidator};
    ValidatedField<int> yearPublished_{yearPublishedValidator};
    ValidatedField<unsigned int> pageNumber_{pageNumberValidator};
    ValidatedField<QString> description_{descriptionValidator};
    ValidatedField<QUrl> thumbnailUrl_{thumbnailUrlValidator};

    /**
     * @brief Constructs a new Book.
     *
     * QUuid and QDate are passed by value since they are trivially copyable.
     */
    Book(QString &&title, QUuid id, QDate dateAdded);

  public:
    ~Book() override = default;
    Book(const Book &) = default;
    Book(Book &&) = default;
    auto operator=(const Book &) -> Book & = default;
    auto operator=(Book &&) -> Book & = default;

    [[nodiscard]] auto operator==(const Book &) const -> bool = default;

    [[nodiscard]] auto clone() const -> std::unique_ptr<Medium> override;

    [[nodiscard]] static auto create(QString title, QUuid id = QUuid::createUuid(),
                                     QDate dateAdded = QDate::currentDate()) -> std::optional<Book>;
    [[nodiscard]] static auto make(QString title, QUuid id = QUuid::createUuid(),
                                   QDate dateAdded = QDate::currentDate())
        -> std::optional<std::unique_ptr<Book>>;

    [[nodiscard]] auto isbn() -> ValidatedField<QString> &;
    [[nodiscard]] auto isbn() const -> const ValidatedField<QString> &;
    [[nodiscard]] static auto isbnValidator(const QString &isbnToValidate) -> bool;

    [[nodiscard]] auto edition() -> ValidatedField<QString> &;
    [[nodiscard]] auto edition() const -> const ValidatedField<QString> &;
    [[nodiscard]] static auto editionValidator(const QString &editionToValidate) -> bool;

    [[nodiscard]] auto publisher() -> ValidatedField<QString> &;
    [[nodiscard]] auto publisher() const -> const ValidatedField<QString> &;
    [[nodiscard]] static auto publisherValidator(const QString &publisherToValidate) -> bool;

    [[nodiscard]] auto yearPublished() -> ValidatedField<int> &;
    [[nodiscard]] auto yearPublished() const -> const ValidatedField<int> &;
    [[nodiscard]] static auto yearPublishedValidator(int yearPublishedToValidate) -> bool;

    [[nodiscard]] auto pageNumber() -> ValidatedField<unsigned int> &;
    [[nodiscard]] auto pageNumber() const -> const ValidatedField<unsigned int> &;
    [[nodiscard]] static auto pageNumberValidator(unsigned int pageNumberToValidate) -> bool;

    [[nodiscard]] auto description() -> ValidatedField<QString> &;
    [[nodiscard]] auto description() const -> const ValidatedField<QString> &;
    [[nodiscard]] static auto descriptionValidator(const QString &descriptionToValidate) -> bool;

    [[nodiscard]] auto thumbnailUrl() -> ValidatedField<QUrl> &;
    [[nodiscard]] auto thumbnailUrl() const -> const ValidatedField<QUrl> &;
    [[nodiscard]] static auto thumbnailUrlValidator(const QUrl &urlToValidate) -> bool;

    void accept(MediumVisitor &visitor) const override;
    void accept(MutMediumVisitor &visitor) override;
};

}

#endif
