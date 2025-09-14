#ifndef STRINGFIELDEDITOR_H
#define STRINGFIELDEDITOR_H

#include "FieldEditor.h"

#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QWidget>
#include <functional>
#include <optional>

namespace Gui::Components {

class StringFieldEditor : public FieldEditor {
    Q_OBJECT

    QLineEdit *editInput;

    std::function<bool(const QString &)> validator;

    bool isOptional;

  public:
    ~StringFieldEditor() override = default;

    StringFieldEditor(const std::function<bool(const QString &)> &newValidator, bool newIsOptional,
                      QWidget *parent);

    StringFieldEditor(const StringFieldEditor &) = delete;
    StringFieldEditor(StringFieldEditor &&) = delete;
    auto operator=(const StringFieldEditor &) -> StringFieldEditor & = delete;
    auto operator=(StringFieldEditor &&) -> StringFieldEditor & = delete;

    void setText(const std::optional<QString> &text);
    [[nodiscard]] auto text() const -> std::optional<QString>;
    [[nodiscard]] auto isValid() const -> bool override;

  private slots:
    void validateInput();
};

}

#endif