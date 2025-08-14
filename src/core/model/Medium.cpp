#include "Medium.h"

namespace Core::Model {

Medium::Medium(QString&& title, const QUuid id, const QDate dateAdded)
    : id_{id}
    , dateAdded_{dateAdded}
    , title_{std::move(title)} {}

bool Medium::createValidator(const QString& title, const QUuid& id, const QDate& dateAdded) {
    return titleValidator(title) && dateAddedValidator(dateAdded) && idValidator(id);
}

Medium::~Medium() = default;

QUuid Medium::id() const {
    return id_;
}
bool Medium::idValidator(const QUuid& idToValidate) {
    return !idToValidate.isNull() && idToValidate.version() == QUuid::Version::Random &&
           idToValidate.variant() == QUuid::Variant::DCE;
}

const MediumUserData& Medium::userData() const {
    return userData_;
}

MediumUserData& Medium::userData() {
    return userData_;
}

QDate Medium::dateAdded() const {
    return dateAdded_;
}

bool Medium::dateAddedValidator(const QDate& dateToValidate) {
    return dateToValidate.isValid();
}

QString Medium::title() const {
    return title_;
}

bool Medium::setTitle(QString titleToSet) {
    if (!titleValidator(titleToSet)) {
        return false;
    }
    title_ = std::move(titleToSet);
    return true;
}

bool Medium::titleValidator(const QString& titleToValidate) {
    return !titleToValidate.trimmed().isEmpty();
}

const ValidatedSet<QString>& Medium::authors() const {
    return authors_;
}
ValidatedSet<QString>& Medium::authors() {
    return authors_;
}
bool Medium::authorValidator(const QString& authorToValidate) {
    return !authorToValidate.trimmed().isEmpty();
}

const ValidatedField<QString>& Medium::language() const {
    return language_;
}
ValidatedField<QString>& Medium::language() {
    return language_;
}
bool Medium::languageValidator(const QString& languageToValidate) {
    return !languageToValidate.trimmed().isEmpty();
}

}