#include "SetFieldEditor.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Gui::Components {

SetFieldEditor::SetFieldEditor(const std::function<bool(const QString &)> &newItemValidator,
                               QWidget *parent)
    : FieldEditor{true, parent}
    , editInput{new QLineEdit{this}}
    , itemValidator{newItemValidator} {

    editInput->setPlaceholderText("Insert a list of comma-separated values");
    setEditModeWidget(editInput);

    setEditMode(false);

    connect(editInput, &QLineEdit::textChanged, this, &SetFieldEditor::validateInput);
    connect(editInput, &QLineEdit::textChanged, this, &SetFieldEditor::stateChanged);
    connect(this, &SetFieldEditor::enabledChanged, this, &SetFieldEditor::validateInput);
    connect(this, &SetFieldEditor::enabledChanged, this, &SetFieldEditor::stateChanged);
}

void SetFieldEditor::setItems(const std::optional<std::set<QString>> &items) {
    if (items.has_value() && !items->empty()) {
        const auto itemList = QStringList(items->begin(), items->end());
        editInput->setText(itemList.join(", "));
        setViewModeText(itemList.join(", "));
        setEnabled(true);
    } else {
        editInput->clear();
        setViewModeText(notSet);
        setEnabled(false);
    }
}

auto SetFieldEditor::items() const -> std::optional<std::set<QString>> {
    if (!isEnabled()) {
        return std::nullopt;
    }
    if (!isValid()) {
        return std::nullopt;
    }
    return lineEditToSet();
}

auto SetFieldEditor::isValid() const -> bool {
    return !isEnabled() || (!lineEditToSet().empty() &&
                            std::ranges::all_of(lineEditToSet(), [this](const auto &item) {
                                return itemValidator(item);
                            }));
}

auto SetFieldEditor::lineEditToSet() const -> std::set<QString> {
    const auto parts = editInput->text().split(',', Qt::SkipEmptyParts);
    std::set<QString> result;
    for (const auto &part : parts) {
        result.insert(part.trimmed());
    }
    return result;
}

void SetFieldEditor::validateInput() {
    if (!isValid()) {
        editInput->setStyleSheet(QString{"background-color: %1;"}.arg(invalidBackground));
    } else {
        editInput->setStyleSheet("");
    }
}

}