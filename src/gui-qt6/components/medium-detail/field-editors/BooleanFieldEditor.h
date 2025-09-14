#ifndef BOOLEANFIELDEDITOR_H
#define BOOLEANFIELDEDITOR_H

#include "FieldEditor.h"

#include <QCheckBox>
#include <QLabel>
#include <QStackedWidget>
#include <QWidget>

namespace Gui::Components {

class BooleanFieldEditor : public FieldEditor {
    Q_OBJECT

    QCheckBox *editInput;

  public:
    ~BooleanFieldEditor() override = default;

    BooleanFieldEditor(QWidget *parent);

    BooleanFieldEditor(const BooleanFieldEditor &) = delete;
    BooleanFieldEditor(BooleanFieldEditor &&) = delete;
    auto operator=(const BooleanFieldEditor &) -> BooleanFieldEditor & = delete;
    auto operator=(BooleanFieldEditor &&) -> BooleanFieldEditor & = delete;

    void setChecked(bool isChecked);
    [[nodiscard]] auto checked() const -> bool;
    [[nodiscard]] auto isValid() const -> bool override;
};

}

#endif