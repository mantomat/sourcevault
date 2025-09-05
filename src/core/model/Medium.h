#ifndef MEDIUM_H
#define MEDIUM_H

#include "MediumUserData.h"
#include "ValidatedField.h"
#include "ValidatedSet.h"
#include "shared/Cloneable.h"
#include "shared/MediumVisitor.h"
#include "shared/MutMediumVisitor.h"

#include <QDate>
#include <QString>
#include <QUuid>

using Core::Shared::Cloneable;
using Core::Shared::MediumVisitor;
using Core::Shared::MutMediumVisitor;

namespace Core::Model {

/**
 * @brief The Medium class represents a medium, such as a book, video, or article.
 * A medium is an item that can be added to the library. Each attribute of a medium is validated.
 */
class Medium : public Cloneable<Medium> {

    QUuid id_;
    QDate dateAdded_;
    QString title_;
    MediumUserData userData_;
    ValidatedSet<QString> authors_{authorValidator};
    ValidatedField<QString> language_{languageValidator};

  protected:
    /*
     * Every Medium maintains the following invariant: it always has a valid title, id and
     * dateAdded.
     *
     * Since the whole system follows an exception-free design, we couldn't just throw an exception
     * at construction time if the parameters were invalid. Hence, we decided to use the static
     * factory method pattern.
     *
     * Note that since factory methods are static, we can't enforce their signature at compile time,
     * but they *must* have the same signature. For a subclass T:
     * `auto create(QString title, QUuid id, QDate dateAdded) -> std::optional<T>`
     * Every subclass T must validate the create() method parameters with `Medium::createValidator`.
     */

    /**
     * @brief Constructs a new Book.
     *
     * QUuid and QDate are passed by value since they are trivially copyable.
     */
    Medium(QString &&title, QUuid id, QDate dateAdded);

    /**
     * @brief Validates factory methods parameters.
     * Every subclass T must validate the create() method parameters with this function.
     */
    static auto createValidator(const QString &title, const QUuid &id, const QDate &dateAdded)
        -> bool;

  public:
    ~Medium() override = default;
    Medium(const Medium &) = default;
    Medium(Medium &&) = default;
    auto operator=(const Medium &) -> Medium & = default;
    auto operator=(Medium &&) -> Medium & = default;

    [[nodiscard]] auto operator==(const Medium &) const -> bool = default;

    [[nodiscard]] auto id() const -> QUuid;
    [[nodiscard]] static auto idValidator(const QUuid &idToValidate) -> bool;

    [[nodiscard]] auto title() const -> QString;
    auto setTitle(QString titleToSet) -> bool;
    [[nodiscard]] static auto titleValidator(const QString &titleToValidate) -> bool;

    [[nodiscard]] auto dateAdded() const -> QDate;
    [[nodiscard]] static auto dateAddedValidator(const QDate &dateToValidate) -> bool;

    [[nodiscard]] auto userData() const -> const MediumUserData &;
    [[nodiscard]] auto userData() -> MediumUserData &;

    [[nodiscard]] auto authors() const -> const ValidatedSet<QString> &;
    [[nodiscard]] auto authors() -> ValidatedSet<QString> &;
    [[nodiscard]] static auto authorValidator(const QString &authorToValidate) -> bool;

    [[nodiscard]] auto language() const -> const ValidatedField<QString> &;
    [[nodiscard]] auto language() -> ValidatedField<QString> &;
    [[nodiscard]] static auto languageValidator(const QString &languageToValidate) -> bool;

    virtual void accept(MediumVisitor &) const = 0;
    virtual void accept(MutMediumVisitor &) = 0;
};

}

#endif
