#include "UserDataDetailSection.h"

#include "components/medium-detail/field-editors/BooleanFieldEditor.h"
#include "components/medium-detail/field-editors/PriorityFieldEditor.h"

#include <QFormLayout>

namespace Gui::Components {

UserDataDetailSection::UserDataDetailSection(const Dependencies &deps, QWidget *parent)
    : QWidget{parent}
    , favoriteEditor{new BooleanFieldEditor{this}}
    , topicsEditor{new SetFieldEditor{deps.topicValidator, this}}
    , notesEditor{new TextFieldEditor{deps.notesValidator, this}}
    , priorityEditor{new PriorityFieldEditor{this}} {

    auto *layout{new QFormLayout{this}};
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addRow("Favorite:", favoriteEditor);
    layout->addRow("Priority:", priorityEditor);
    layout->addRow("Topics:", topicsEditor);
    layout->addRow("Notes:", notesEditor);

    connect(favoriteEditor, &BooleanFieldEditor::stateChanged, this,
            &UserDataDetailSection::stateChanged);
    connect(topicsEditor, &SetFieldEditor::stateChanged, this,
            &UserDataDetailSection::stateChanged);
    connect(notesEditor, &TextFieldEditor::stateChanged, this,
            &UserDataDetailSection::stateChanged);
    connect(priorityEditor, &PriorityFieldEditor::stateChanged, this,
            &UserDataDetailSection::stateChanged);
}

auto UserDataDetailSection::isEverythingValid() const -> bool {
    return favoriteEditor->isValid() && topicsEditor->isValid() && notesEditor->isValid() &&
           priorityEditor->isValid();
}

auto UserDataDetailSection::getState() const -> UserDataDetailViewModel {
    return UserDataDetailViewModel{
        .favorite = favoriteEditor->checked(),
        .topics = topicsEditor->items(),
        .notes = notesEditor->text(),
        .priority = priorityEditor->priority(),
    };
}

void UserDataDetailSection::setState(const UserDataDetailViewModel &initData) {
    favoriteEditor->setChecked(initData.favorite);
    topicsEditor->setItems(initData.topics);
    notesEditor->setText(initData.notes);
    priorityEditor->setPriority(initData.priority);
}

void UserDataDetailSection::setEditMode(bool isEditing) {
    favoriteEditor->setEditMode(isEditing);
    topicsEditor->setEditMode(isEditing);
    notesEditor->setEditMode(isEditing);
    priorityEditor->setEditMode(isEditing);
}

}