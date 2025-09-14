#ifndef MEDIUMSECTIONCONTROLLER_H
#define MEDIUMSECTIONCONTROLLER_H

#include "MediumDetailSection.h"
#include "model/Medium.h"

#include <QObject>

using Core::Model::Medium;

namespace Gui::Components {

class MediumSectionController : public QObject {
    Q_OBJECT

    MediumDetailSection *section;

  public:
    ~MediumSectionController() override = default;

    MediumSectionController(MediumDetailSection *newSection, const Medium *mediumToDisplay,
                            QObject *parent);

    MediumSectionController(const MediumSectionController &) = delete;
    MediumSectionController(MediumSectionController &&) = delete;
    auto operator=(const MediumSectionController &) -> MediumSectionController & = delete;
    auto operator=(MediumSectionController &&) -> MediumSectionController & = delete;

    void applyChanges(Medium &mediumToModify) const;

  private:
    void populateSection(const Medium *mediumToDisplay);
};

}

#endif