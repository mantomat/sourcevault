#include "TextFieldEditor.h"

#include <QVBoxLayout>

namespace Gui::Components {

TextFieldEditor::TextFieldEditor(const std::function<bool(const QString &)> &newValidator,
                                 QWidget *parent)
    : FieldEditor{true, parent}
    , editInput{new QTextEdit{this}}
    , validator{newValidator} {

    setEditModeWidget(editInput);
    setEditMode(false);

    connect(editInput, &QTextEdit::textChanged, this, &TextFieldEditor::validateInput);
    connect(editInput, &QTextEdit::textChanged, this, &TextFieldEditor::stateChanged);
    connect(this, &TextFieldEditor::enabledChanged, this, &TextFieldEditor::stateChanged);
    connect(this, &TextFieldEditor::enabledChanged, this, &TextFieldEditor::validateInput);
}

void TextFieldEditor::setText(const std::optional<QString> &text) {
    setViewModeText(text.value_or(notSet));
    editInput->setText(text.value_or(""));
    setEnabled(text.has_value());
    validateInput();
}

auto TextFieldEditor::text() const -> std::optional<QString> {
    if (!isValid()) {
        return std::nullopt;
    }
    return editInput->toPlainText();
}

auto TextFieldEditor::isValid() const -> bool {
    const QString currentText{editInput->toPlainText()};
    return !isEnabled() || validator(currentText);
}

void TextFieldEditor::validateInput() {
    if (!isValid()) {
        editInput->setStyleSheet(QString{"background-color: %1;"}.arg(invalidBackground));
    } else {
        editInput->setStyleSheet("");
    }
}

}