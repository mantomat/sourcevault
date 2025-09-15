#include "MediumDetailSection.h"

#include "components/medium-detail/field-editors/SetFieldEditor.h"
#include "components/medium-detail/field-editors/StringFieldEditor.h"

#include <QFormLayout>

namespace Gui::Components {

MediumDetailSection::MediumDetailSection(const Dependencies &deps, QWidget *parent)
    : QWidget{parent}
    , titleEditor{new StringFieldEditor{deps.titleValidator, false, this}}
    , authorsEditor{new SetFieldEditor{deps.authorValidator, this}}
    , languageEditor{new StringFieldEditor{deps.languageValidator, true, this}}
    , dateAddedLabel{new QLabel{this}}
    , idLabel{new QLabel{this}} {

    auto *layout{new QFormLayout{this}};
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addRow("Title:", titleEditor);
    layout->addRow("Added on:", dateAddedLabel);
    layout->addRow("Id:", idLabel);
    layout->addRow("Authors:", authorsEditor);
    layout->addRow("Language:", languageEditor);

    connect(titleEditor, &StringFieldEditor::stateChanged, this,
            &MediumDetailSection::stateChanged);
    connect(authorsEditor, &SetFieldEditor::stateChanged, this, &MediumDetailSection::stateChanged);
    connect(languageEditor, &StringFieldEditor::stateChanged, this,
            &MediumDetailSection::stateChanged);
}

auto MediumDetailSection::isEverythingValid() const -> bool {
    return titleEditor->isValid() && authorsEditor->isValid() && languageEditor->isValid();
}

auto MediumDetailSection::getState() const -> MediumDetailViewModel {
    return MediumDetailViewModel{.title = titleEditor->text().value(),
                                 .dateAdded = QDate{},
                                 .id = QUuid{},
                                 .authors = authorsEditor->items(),
                                 .language = languageEditor->text()};
}

void MediumDetailSection::setState(const MediumDetailViewModel &initData) {
    titleEditor->setText(initData.title);
    dateAddedLabel->setText(initData.dateAdded.toString("yyyy-MM-dd"));
    idLabel->setText(initData.id.toString(QUuid::WithoutBraces));
    authorsEditor->setItems(initData.authors);
    languageEditor->setText(initData.language);
}

void MediumDetailSection::setEditMode(bool isEditing) {
    titleEditor->setEditMode(isEditing);
    authorsEditor->setEditMode(isEditing);
    languageEditor->setEditMode(isEditing);
}

}