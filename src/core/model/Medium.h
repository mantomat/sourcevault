#ifndef MEDIUM_H
#define MEDIUM_H

#include "MediumUserData.h"
#include "ValidatedField.h"
#include "ValidatedSet.h"

#include <QDate>
#include <QString>
#include <set>

namespace Core::Model {

/**
 * @brief The Medium class represents a medium, such as a book, video, or article.
 *
 * A medium is an item that can be added to the library. Each attribute of a medium is validated.
 *
 * Note for the professor: I am aware that this class design (and its hierarchy) breaks the
 * traditional object-oriented principle of encapsulation by exposing direct access to internal
 * fields. However, this was a deliberate and thoughtful design decision.
 *
 * In this implementation, a Medium is conceptualized more as a data structure than as a
 * behavior-rich object. Its primary role is to hold and organize data, while all validation logic
 * and invariants are delegated to wrapper classes such as ValidatedField and ValidatedSet. Think
 * about this class as something like a Record in Java.
 *
 * The original version relied heavily on boilerplate methods (getX, hasX, setX, unsetX, ...)
 * that simply forwarded calls to the respective ValidatedField methods. Since the interfaces of
 * Medium attributes and their wrappers were nearly identical, maintaining such boilerplate offered
 * no real encapsulation benefits and only made the codebase harder to maintain and evolve.
 *
 * Another advantage of this approach is the elision of the need for the builder pattern. Users can
 * just create an empty Medium, set the attributes they want and only use it when they need to.
 *
 * I recognize the tradeoffs of this design:
 * - Reduced encapsulation: switching to a different field-wrapping strategy would require interface
 * changes.
 * - Difficulty in enforcing invariants across multiple fields (e.g., ensuring that dateAdded <
 * dateFinished).
 *
 * Nonetheless, I believe this approach results in a cleaner, more flexible, and maintainable design
 * for a data-centric class like Medium.
 */
class Medium {

  public:
    virtual ~Medium() = 0;

    const MediumUserData& userData() const;
    MediumUserData& userData();

    const ValidatedField<QDate>& dateAdded() const;
    ValidatedField<QDate>& dateAdded();
    static bool dateAddedValidator(const QDate& dateToValidate);

    const ValidatedField<QString>& title() const;
    ValidatedField<QString>& title();
    static bool titleValidator(const QString& titleToValidate);

    const ValidatedSet<QString>& authors() const;
    ValidatedSet<QString>& authors();
    static bool authorValidator(const QString& authorToValidate);

    const ValidatedField<QString>& language() const;
    ValidatedField<QString>& language();
    static bool languageValidator(const QString& languageToValidate);

  private:
    MediumUserData userData_;
    ValidatedField<QDate> dateAdded_{dateAddedValidator};
    ValidatedField<QString> title_{titleValidator};
    ValidatedSet<QString> authors_{authorValidator};
    ValidatedField<QString> language_{languageValidator};
};

}

#endif
