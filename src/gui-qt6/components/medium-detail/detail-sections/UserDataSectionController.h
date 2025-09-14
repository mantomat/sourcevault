#ifndef USERDATASECTIONCONTROLLER_H
#define USERDATASECTIONCONTROLLER_H

#include "UserDataDetailSection.h"
#include "model/Medium.h"

#include <QObject>

using Core::Model::Medium;
using Core::Model::MediumUserData;

namespace Gui::Components {

class UserDataSectionController : public QObject {
    Q_OBJECT

    UserDataDetailSection *section;

  public:
    ~UserDataSectionController() override = default;

    UserDataSectionController(UserDataDetailSection *newSection, const Medium *mediumToDisplay,
                              QObject *parent);

    UserDataSectionController(const UserDataSectionController &) = delete;
    UserDataSectionController(UserDataSectionController &&) = delete;
    auto operator=(const UserDataSectionController &) -> UserDataSectionController & = delete;
    auto operator=(UserDataSectionController &&) -> UserDataSectionController & = delete;

    void applyChanges(Medium &mediumToModify) const;

  private:
    void populateSection(const Medium *mediumToDisplay);

    static auto viewModelToPriority(std::optional<PriorityFieldEditor::Priority> viewModel)
        -> std::optional<MediumUserData::PriorityLevel>;
    static auto priorityToViewModel(std::optional<MediumUserData::PriorityLevel> priority)
        -> std::optional<PriorityFieldEditor::Priority>;
};

}

#endif