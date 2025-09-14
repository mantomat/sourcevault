#include "BooleanFieldEditor.h"

#include <QVBoxLayout>
#include <qnamespace.h>

namespace Gui::Components {

BooleanFieldEditor::BooleanFieldEditor(QWidget *parent)
    : FieldEditor{false, parent}
    , editInput{new QCheckBox{this}} {

    setEditModeWidget(editInput);
    setEditMode(false);

    connect(editInput, &QCheckBox::stateChanged, this, &BooleanFieldEditor::stateChanged);
}

void BooleanFieldEditor::setChecked(bool isChecked) {
    editInput->setChecked(isChecked);
    setViewModeText(isChecked ? QString::fromUtf8("\u2713") : QString::fromUtf8("\u2717"));
}

auto BooleanFieldEditor::checked() const -> bool {
    return editInput->isChecked();
}

auto BooleanFieldEditor::isValid() const -> bool {
    return true;
}

}