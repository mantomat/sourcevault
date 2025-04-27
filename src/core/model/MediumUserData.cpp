#include "MediumUserData.h"

namespace Core::Model {
bool& MediumUserData::favorite() {
    return favorite_;
}
const bool& MediumUserData::favorite() const {
    return favorite_;
}

ValidatedSet<QString>& MediumUserData::topics() {
    return topics_;
}
const ValidatedSet<QString>& MediumUserData::topics() const {
    return topics_;
}
bool MediumUserData::topicValidator(const QString& topicToValidate) {
    return !topicToValidate.trimmed().isEmpty();
}

ValidatedField<QString>& MediumUserData::notes() {
    return notes_;
}
const ValidatedField<QString>& MediumUserData::notes() const {
    return notes_;
}
bool MediumUserData::notesValidator(const QString& notesToValidate) {
    return !notesToValidate.trimmed().isEmpty();
}

OptionalField<MediumUserData::PriorityLevel>& MediumUserData::priority() {
    return priority_;
}
const OptionalField<MediumUserData::PriorityLevel>& MediumUserData::priority() const {
    return priority_;
}

}