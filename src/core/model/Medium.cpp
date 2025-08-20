#include "Medium.h"

namespace Core::Model {

Medium::Medium(QString &&title, const QUuid id, const QDate dateAdded)
    : id_{id}
    , dateAdded_{dateAdded}
    , title_{std::move(title)} {}

auto Medium::createValidator(const QString &title, const QUuid &id, const QDate &dateAdded)
    -> bool {
    return titleValidator(title) && dateAddedValidator(dateAdded) && idValidator(id);
}

auto Medium::id() const -> QUuid {
    return id_;
}
auto Medium::idValidator(const QUuid &idToValidate) -> bool {
    return !idToValidate.isNull() && idToValidate.version() == QUuid::Version::Random &&
           idToValidate.variant() == QUuid::Variant::DCE;
}

auto Medium::userData() const -> const MediumUserData & {
    return userData_;
}

auto Medium::userData() -> MediumUserData & {
    return userData_;
}

auto Medium::dateAdded() const -> QDate {
    return dateAdded_;
}

auto Medium::dateAddedValidator(const QDate &dateToValidate) -> bool {
    return dateToValidate.isValid();
}

auto Medium::title() const -> QString {
    return title_;
}

auto Medium::setTitle(QString titleToSet) -> bool {
    if (!titleValidator(titleToSet)) {
        return false;
    }
    title_ = std::move(titleToSet);
    return true;
}

auto Medium::titleValidator(const QString &titleToValidate) -> bool {
    return !titleToValidate.trimmed().isEmpty();
}

auto Medium::authors() const -> const ValidatedSet<QString> & {
    return authors_;
}
auto Medium::authors() -> ValidatedSet<QString> & {
    return authors_;
}
auto Medium::authorValidator(const QString &authorToValidate) -> bool {
    return !authorToValidate.trimmed().isEmpty();
}

auto Medium::language() const -> const ValidatedField<QString> & {
    return language_;
}
auto Medium::language() -> ValidatedField<QString> & {
    return language_;
}
auto Medium::languageValidator(const QString &languageToValidate) -> bool {
    return !languageToValidate.trimmed().isEmpty();
}

}