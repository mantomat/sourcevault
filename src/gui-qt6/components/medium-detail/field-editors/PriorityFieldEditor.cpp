#include "PriorityFieldEditor.h"

#include <QVBoxLayout>

namespace Gui::Components {

PriorityFieldEditor::PriorityFieldEditor(QWidget *parent)
    : FieldEditor{true, parent}
    , editInput{new QComboBox{this}} {

    initEditor();

    setEditModeWidget(editInput);
    setEditMode(false);

    connect(editInput, &QComboBox::currentIndexChanged, this, &PriorityFieldEditor::stateChanged);
}

void PriorityFieldEditor::setPriority(std::optional<Priority> priority) {
    if (priority.has_value()) {
        int index{editInput->findData(QVariant::fromValue(priority.value()))};
        editInput->setCurrentIndex(index);

        setViewModeText(editInput->itemText(index));
        setEnabled(true);
    } else {
        int index{editInput->findData(QVariant::fromValue(Priority::Mid))};
        editInput->setCurrentIndex(index);

        setViewModeText(notSet);
        setEnabled(false);
    }
}

auto PriorityFieldEditor::priority() const -> std::optional<Priority> {
    return isEnabled() ? editInput->currentData().value<Priority>()
                       : std::optional<Priority>{std::nullopt};
}

auto PriorityFieldEditor::isValid() const -> bool {
    return true;
}

void PriorityFieldEditor::initEditor() {
    editInput->addItem("Minimum", QVariant::fromValue(Priority::Min));
    editInput->addItem("Low", QVariant::fromValue(Priority::Low));
    editInput->addItem("Medium", QVariant::fromValue(Priority::Mid));
    editInput->addItem("High", QVariant::fromValue(Priority::High));
    editInput->addItem("Maximum", QVariant::fromValue(Priority::Max));
}

}