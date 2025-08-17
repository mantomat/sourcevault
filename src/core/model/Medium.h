#ifndef MEDIUM_H
#define MEDIUM_H

#include "MediumUserData.h"
#include "ValidatedField.h"
#include "ValidatedSet.h"

#include <QDate>
#include <QString>
#include <QUuid>

namespace Core::Model {

/**
 * @brief The Medium class represents a medium, such as a book, video, or article.
 * A medium is an item that can be added to the library. Each attribute of a medium is validated.
 */
class Medium {

    const QUuid id_;
    const QDate dateAdded_;
    QString title_;
    MediumUserData userData_;
    ValidatedSet<QString> authors_{authorValidator};
    ValidatedField<QString> language_{languageValidator};

  protected:
    /*
     * Invariant: a Medium must always have a valid title, id and dateAdded.
     * These methods are protected since they are used for construction through subclasses
     * QUuid and QDate are trivially copyable
     */
    Medium(QString&& title, QUuid id, QDate dateAdded);
    static bool createValidator(const QString& title, const QUuid& id, const QDate& dateAdded);

  public:
    Medium(const Medium&) = default;
    Medium(Medium&&) = default;
    virtual ~Medium() = 0;

    auto operator==(const Medium&) const -> bool = default;

    virtual std::unique_ptr<Medium> clone() const = 0;

    QUuid id() const;
    static bool idValidator(const QUuid& idToValidate);

    QString title() const;
    bool setTitle(QString titleToSet);
    static bool titleValidator(const QString& titleToValidate);

    QDate dateAdded() const;
    static bool dateAddedValidator(const QDate& dateToValidate);

    const MediumUserData& userData() const;
    MediumUserData& userData();

    const ValidatedSet<QString>& authors() const;
    ValidatedSet<QString>& authors();
    static bool authorValidator(const QString& authorToValidate);

    const ValidatedField<QString>& language() const;
    ValidatedField<QString>& language();
    static bool languageValidator(const QString& languageToValidate);
};

}

#endif
