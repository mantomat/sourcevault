#ifndef USERINFO_H
#define USERINFO_H

#include "ValidatedField.h"
#include "ValidatedSet.h"

#include <QString>

namespace Core::Model {

class MediumUserData {

  public:
    enum class PriorityLevel : std::uint8_t {
        min = 1,
        low = 2,
        mid = 3,
        high = 4,
        max = 5,
    };

  private:
    bool favorite_{false};
    ValidatedSet<QString> topics_{topicValidator};
    ValidatedField<QString> notes_{notesValidator};
    ValidatedField<PriorityLevel> priority_{priorityValidator};

  public:
    [[nodiscard]] auto operator==(const MediumUserData &) const -> bool = default;

    [[nodiscard]] auto favorite() -> bool &;
    [[nodiscard]] auto favorite() const -> bool;

    [[nodiscard]] auto topics() -> ValidatedSet<QString> &;
    [[nodiscard]] auto topics() const -> const ValidatedSet<QString> &;
    [[nodiscard]] static auto topicValidator(const QString &topicToValidate) -> bool;

    [[nodiscard]] auto notes() -> ValidatedField<QString> &;
    [[nodiscard]] auto notes() const -> const ValidatedField<QString> &;
    [[nodiscard]] static auto notesValidator(const QString &notesToValidate) -> bool;

    [[nodiscard]] auto priority() -> ValidatedField<PriorityLevel> &;
    [[nodiscard]] auto priority() const -> const ValidatedField<PriorityLevel> &;
    [[nodiscard]] static auto priorityValidator(PriorityLevel priorityToValidate) -> bool;
};

}

#endif
