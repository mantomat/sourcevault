#include "NumberFieldEditor.h"

#include <QVBoxLayout>

namespace Gui::Components {

NumberFieldEditor::NumberFieldEditor(const std::function<bool(int)> &newValidator, QWidget *parent)
    : FieldEditor{true, parent}
    , editInput{new QSpinBox{this}}
    , validator{newValidator} {

    setEditModeWidget(editInput);
    setEditMode(false);

    connect(editInput, &QSpinBox::valueChanged, this, &NumberFieldEditor::validateInput);
    connect(editInput, &QSpinBox::valueChanged, this, &NumberFieldEditor::stateChanged);
    connect(this, &NumberFieldEditor::enabledChanged, this, &NumberFieldEditor::stateChanged);
    connect(this, &NumberFieldEditor::enabledChanged, this, &NumberFieldEditor::validateInput);
}

void NumberFieldEditor::setNumber(const std::optional<int> &number) {
    if (number.has_value()) {
        setViewModeText(QString::number(number.value()));
        editInput->setValue(number.value());
        setEnabled(true);
    } else {
        setViewModeText(notSet);
        editInput->setValue(1);
        setEnabled(false);
    }
    validateInput();
}

auto NumberFieldEditor::number() const -> std::optional<int> {
    if (!isValid()) {
        return std::nullopt;
    }
    return editInput->value();
}

auto NumberFieldEditor::isValid() const -> bool {
    return !isEnabled() || validator(editInput->value());
}

void NumberFieldEditor::validateInput() {
    if (!isValid()) {
        editInput->setStyleSheet(QString{"background-color: %1;"}.arg(invalidBackground));
    } else {
        editInput->setStyleSheet("");
    }
}

}