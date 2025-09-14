#include "StringFieldEditor.h"

#include <QVBoxLayout>

namespace Gui::Components {

StringFieldEditor::StringFieldEditor(const std::function<bool(const QString &)> &newValidator,
                                     bool newIsOptional, QWidget *parent)
    : FieldEditor{newIsOptional, parent}
    , editInput{new QLineEdit{this}}
    , validator{newValidator}
    , isOptional{newIsOptional} {

    setEditModeWidget(editInput);
    setEditMode(false);

    connect(editInput, &QLineEdit::textChanged, this, &StringFieldEditor::validateInput);
    connect(editInput, &QLineEdit::textChanged, this, &StringFieldEditor::stateChanged);
    connect(this, &StringFieldEditor::enabledChanged, this, &StringFieldEditor::validateInput);
}

void StringFieldEditor::setText(const std::optional<QString> &text) {
    setViewModeText(text.value_or(notSet));
    editInput->setText(text.value_or(""));
    setEnabled(text.has_value());
    validateInput();
}

auto StringFieldEditor::text() const -> std::optional<QString> {
    if (!isValid()) {
        return std::nullopt;
    }
    return editInput->text();
}

auto StringFieldEditor::isValid() const -> bool {
    return !isEnabled() || validator(editInput->text());
}

void StringFieldEditor::validateInput() {
    if (!isValid()) {
        editInput->setStyleSheet(QString{"background-color: %1;"}.arg(invalidBackground));
    } else {
        editInput->setStyleSheet("");
    }
}

}