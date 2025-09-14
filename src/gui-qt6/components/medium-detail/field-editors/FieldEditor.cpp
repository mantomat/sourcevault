#include "FieldEditor.h"

#include <QVBoxLayout>

namespace Gui::Components {

FieldEditor::FieldEditor(bool optional, QWidget *parent)
    : QWidget{parent}
    , stack{new QStackedWidget{this}}
    , editWidget{nullptr}
    , editWidgetContainer{nullptr}
    , viewLabel{new QLabel{notSet, this}}
    , enabledCheckBox{new QCheckBox{this}} {

    auto *layout{new QVBoxLayout{this}};
    layout->setContentsMargins(0, 0, 0, 0);

    viewLabel->setWordWrap(true);
    stack->addWidget(viewLabel);
    layout->addWidget(stack);

    if (!optional) {
        enabledCheckBox->setChecked(true);
    }
    enabledCheckBox->setVisible(optional);

    connect(enabledCheckBox, &QCheckBox::clicked, this, &FieldEditor::setEnabled);
}

auto FieldEditor::isEnabled() const -> bool {
    return enabledCheckBox->isChecked();
}

void FieldEditor::setViewModeText(const QString &text) {
    viewLabel->setText(text);
}

void FieldEditor::setEditModeWidget(QWidget *newEditWidget) {
    editWidget = newEditWidget;

    editWidgetContainer = new QWidget{this};
    auto *layout{new QHBoxLayout{editWidgetContainer}};
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(enabledCheckBox);
    layout->addWidget(newEditWidget, 1);

    stack->addWidget(editWidgetContainer);
}

void FieldEditor::setEditMode(bool isEditing) {
    stack->setCurrentWidget(isEditing ? editWidgetContainer : static_cast<QWidget *>(viewLabel));
    setFixedHeight(isEditing ? editWidgetContainer->sizeHint().height()
                             : viewLabel->sizeHint().height());
}

void FieldEditor::setEnabled(bool enabled) {
    enabledCheckBox->setChecked(enabled);
    editWidget->setEnabled(enabled);
    emit enabledChanged();
}

}
