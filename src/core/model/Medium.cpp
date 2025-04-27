#include "Medium.h"

namespace Core::Model {

Medium::~Medium() = default;

const MediumUserData& Medium::userData() const {
    return userData_;
}
MediumUserData& Medium::userData() {
    return userData_;
}
const ValidatedField<QDate>& Medium::dateAdded() const {
    return dateAdded_;
}
ValidatedField<QDate>& Medium::dateAdded() {
    return dateAdded_;
}
bool Medium::dateAddedValidator(const QDate& dateToValidate) {
    return dateToValidate.isValid();
}

const ValidatedField<QString>& Medium::title() const {
    return title_;
}
ValidatedField<QString>& Medium::title() {
    return title_;
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