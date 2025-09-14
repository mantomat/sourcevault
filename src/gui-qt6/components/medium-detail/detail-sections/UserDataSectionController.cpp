#include "UserDataSectionController.h"

#include "UserDataDetailSection.h"
#include "components/medium-detail/field-editors/PriorityFieldEditor.h"
#include "model/MediumUserData.h"

namespace Gui::Components {

UserDataSectionController::UserDataSectionController(UserDataDetailSection *newSection,
                                                     const Medium *mediumToDisplay, QObject *parent)
    : QObject{parent}
    , section{newSection} {
    assert(newSection != nullptr);

    populateSection(mediumToDisplay);
}

void UserDataSectionController::applyChanges(Medium &medium) const {
    assert(section->isEverythingValid());

    const auto formData{section->getState()};

    medium.userData().favorite() = formData.favorite;
    if (formData.notes.has_value()) {
        medium.userData().notes().set(formData.notes.value());
    } else {
        medium.userData().notes().unset();
    }

    if (formData.priority.has_value()) {
        medium.userData().priority().set(viewModelToPriority(formData.priority).value());
    } else {
        medium.userData().priority().unset();
    }

    if (formData.topics.has_value()) {
        medium.userData().topics().set(formData.topics.value());
    } else {
        medium.userData().topics().unset();
    }
}

void UserDataSectionController::populateSection(const Medium *mediumToDisplay) {
    section->setState({
        .favorite = mediumToDisplay->userData().favorite(),
        .topics = mediumToDisplay->userData().topics().get(),
        .notes = mediumToDisplay->userData().notes().get(),
        .priority = priorityToViewModel(mediumToDisplay->userData().priority().get()),
    });
}

auto UserDataSectionController::viewModelToPriority(
    std::optional<PriorityFieldEditor::Priority> viewModel)
    -> std::optional<MediumUserData::PriorityLevel> {

    if (!viewModel.has_value()) {
        return std::nullopt;
    }

    switch (viewModel.value()) {
    case PriorityFieldEditor::Priority::Min:
        return MediumUserData::PriorityLevel::min;
    case PriorityFieldEditor::Priority::Low:
        return MediumUserData::PriorityLevel::low;
    case PriorityFieldEditor::Priority::Mid:
        return MediumUserData::PriorityLevel::mid;
    case PriorityFieldEditor::Priority::High:
        return MediumUserData::PriorityLevel::high;
    case PriorityFieldEditor::Priority::Max:
        return MediumUserData::PriorityLevel::max;
    }

    return std::nullopt;
}

auto UserDataSectionController::priorityToViewModel(
    std::optional<MediumUserData::PriorityLevel> priority)
    -> std::optional<PriorityFieldEditor::Priority> {
    if (!priority.has_value()) {
        return std::nullopt;
    }

    switch (priority.value()) {
    case Core::Model::MediumUserData::PriorityLevel::min:
        return PriorityFieldEditor::Priority::Min;
    case Core::Model::MediumUserData::PriorityLevel::low:
        return PriorityFieldEditor::Priority::Low;
    case Core::Model::MediumUserData::PriorityLevel::mid:
        return PriorityFieldEditor::Priority::Mid;
    case Core::Model::MediumUserData::PriorityLevel::high:
        return PriorityFieldEditor::Priority::High;
    case Core::Model::MediumUserData::PriorityLevel::max:
        return PriorityFieldEditor::Priority::Max;
    }

    return std::nullopt;
}

}