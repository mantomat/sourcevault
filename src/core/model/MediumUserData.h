#ifndef USERINFO_H
#define USERINFO_H

#include "OptionalField.h"
#include "ValidatedField.h"
#include "ValidatedSet.h"

#include <QString>

namespace Core::Model {

class MediumUserData {
public:
    enum class PriorityLevel : unsigned short {
        min = 1,
        low = 2,
        mid = 3,
        high = 4,
        max = 5,
    };

    bool& favorite();
    const bool& favorite() const;

    ValidatedSet<QString>& topics();
    const ValidatedSet<QString>& topics() const;
    static bool topicValidator(const QString& topicToValidate);

    ValidatedField<QString>& notes();
    const ValidatedField<QString>& notes() const;
    static bool notesValidator(const QString& notesToValidate);

    OptionalField<PriorityLevel>& priority();
    const OptionalField<PriorityLevel>& priority() const;

private:
    bool favorite_{false};
    ValidatedSet<QString> topics_{topicValidator};
    ValidatedField<QString> notes_{notesValidator};
    OptionalField<PriorityLevel> priority_;
};

}

#endif
