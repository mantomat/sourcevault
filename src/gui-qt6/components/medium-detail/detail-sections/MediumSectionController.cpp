#include "MediumSectionController.h"

namespace Gui::Components {

MediumSectionController::MediumSectionController(MediumDetailSection *newSection,
                                                 const Medium *mediumToDisplay, QObject *parent)
    : QObject{parent}
    , section{newSection} {
    populateSection(mediumToDisplay);
}

void MediumSectionController::applyChanges(Medium &mediumToModify) const {
    assert(section->isEverythingValid());

    const auto formData{section->getState()};

    mediumToModify.setTitle(formData.title);
    if (formData.authors.has_value()) {
        mediumToModify.authors().set(formData.authors.value());
    } else {
        mediumToModify.authors().unset();
    }

    if (formData.language.has_value()) {
        mediumToModify.language().set(formData.language.value());
    } else {
        mediumToModify.language().unset();
    }
}

void MediumSectionController::populateSection(const Medium *mediumToDisplay) {
    section->setState({
        .title = mediumToDisplay->title(),
        .authors = mediumToDisplay->authors().get(),
        .language = mediumToDisplay->language().get(),
    });
}

}