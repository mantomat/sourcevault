#ifndef USERINFO_H
#define USERINFO_H

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
    bool favorite() const;

    ValidatedSet<QString>& topics();
    const ValidatedSet<QString>& topics() const;
    static bool topicValidator(const QString& topicToValidate);

    ValidatedField<QString>& notes();
    const ValidatedField<QString>& notes() const;
    static bool notesValidator(const QString& notesToValidate);

    ValidatedField<PriorityLevel>& priority();
    const ValidatedField<PriorityLevel>& priority() const;
    static bool priorityValidator(PriorityLevel priorityToValidate);

  private:
    bool favorite_{false};
    ValidatedSet<QString> topics_{topicValidator};
    ValidatedField<QString> notes_{notesValidator};
    ValidatedField<PriorityLevel> priority_{priorityValidator};
};

}

#endif
