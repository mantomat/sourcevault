#include "MediumUserData.h"

namespace Core::Model {
bool& MediumUserData::favorite() {
    return favorite_;
}
bool MediumUserData::favorite() const {
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

ValidatedField<MediumUserData::PriorityLevel>& MediumUserData::priority() {
    return priority_;
}
const ValidatedField<MediumUserData::PriorityLevel>& MediumUserData::priority() const {
    return priority_;
}
bool MediumUserData::priorityValidator(PriorityLevel priorityToValidate) {
    const auto underlyingValue =
        static_cast<std::underlying_type_t<PriorityLevel>>(priorityToValidate);

    constexpr auto min = static_cast<std::underlying_type_t<PriorityLevel>>(PriorityLevel::min);
    constexpr auto max = static_cast<std::underlying_type_t<PriorityLevel>>(PriorityLevel::max);

    return underlyingValue >= min && underlyingValue <= max;
}
}