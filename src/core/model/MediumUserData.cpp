#include "MediumUserData.h"

namespace Core::Model {

auto MediumUserData::favorite() -> bool & {
    return favorite_;
}
auto MediumUserData::favorite() const -> bool {
    return favorite_;
}

auto MediumUserData::topics() -> ValidatedSet<QString> & {
    return topics_;
}
auto MediumUserData::topics() const -> const ValidatedSet<QString> & {
    return topics_;
}
auto MediumUserData::topicValidator(const QString &topicToValidate) -> bool {
    return !topicToValidate.trimmed().isEmpty();
}

auto MediumUserData::notes() -> ValidatedField<QString> & {
    return notes_;
}
auto MediumUserData::notes() const -> const ValidatedField<QString> & {
    return notes_;
}
auto MediumUserData::notesValidator(const QString &notesToValidate) -> bool {
    return !notesToValidate.trimmed().isEmpty();
}

auto MediumUserData::priority() -> ValidatedField<MediumUserData::PriorityLevel> & {
    return priority_;
}
auto MediumUserData::priority() const -> const ValidatedField<MediumUserData::PriorityLevel> & {
    return priority_;
}
auto MediumUserData::priorityValidator(PriorityLevel priorityToValidate) -> bool {
    const auto underlyingValue =
        static_cast<std::underlying_type_t<PriorityLevel>>(priorityToValidate);

    constexpr auto min = static_cast<std::underlying_type_t<PriorityLevel>>(PriorityLevel::min);
    constexpr auto max = static_cast<std::underlying_type_t<PriorityLevel>>(PriorityLevel::max);

    return underlyingValue >= min && underlyingValue <= max;
}

}