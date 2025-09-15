#include "DateFieldEditor.h"

#include <QVBoxLayout>

namespace Gui::Components {

DateFieldEditor::DateFieldEditor(const std::function<bool(const QDate &)> &newValidator,
                                 QWidget *parent)
    : FieldEditor{true, parent}
    , editInput{new QDateEdit{this}}
    , validator{newValidator} {

    setEditModeWidget(editInput);
    setEditMode(false);

    editInput->setDisplayFormat(dateFormat);
    editInput->setCalendarPopup(true);

    connect(editInput, &QDateEdit::dateChanged, this, &DateFieldEditor::validateInput);
    connect(editInput, &QDateEdit::dateChanged, this, &DateFieldEditor::stateChanged);
    connect(this, &DateFieldEditor::enabledChanged, this, &DateFieldEditor::stateChanged);
    connect(this, &DateFieldEditor::enabledChanged, this, &DateFieldEditor::validateInput);
}

void DateFieldEditor::setDate(const std::optional<QDate> &date) {
    if (date.has_value()) {
        setViewModeText(date->toString(dateFormat));
        editInput->setDate(date.value());
        setEnabled(true);
    } else {
        setViewModeText(notSet);
        editInput->setDate(QDate::currentDate());
        setEnabled(false);
    }
    validateInput();
}

auto DateFieldEditor::date() const -> std::optional<QDate> {
    if (!isEnabled()) {
        return std::nullopt;
    }
    if (!isValid()) {
        return std::nullopt;
    }
    return editInput->date();
}

auto DateFieldEditor::isValid() const -> bool {
    return !isEnabled() || validator(editInput->date());
}

void DateFieldEditor::validateInput() {
    if (!isValid()) {
        editInput->setStyleSheet(QString{"background-color: %1;"}.arg(invalidBackground));
    } else {
        editInput->setStyleSheet("");
    }
}

}