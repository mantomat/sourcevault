#ifndef TEXTFIELDEDITOR_H
#define TEXTFIELDEDITOR_H

#include "FieldEditor.h"

#include <QLabel>
#include <QTextEdit>

namespace Gui::Components {

class TextFieldEditor : public FieldEditor {
    Q_OBJECT

    QTextEdit *editInput;

    std::function<bool(const QString &)> validator;

  public:
    ~TextFieldEditor() override = default;

    TextFieldEditor(const std::function<bool(const QString &)> &newValidator, QWidget *parent);

    TextFieldEditor(const TextFieldEditor &) = delete;
    TextFieldEditor(TextFieldEditor &&) = delete;
    auto operator=(const TextFieldEditor &) -> TextFieldEditor & = delete;
    auto operator=(TextFieldEditor &&) -> TextFieldEditor & = delete;

    void setText(const std::optional<QString> &text);
    [[nodiscard]] auto text() const -> std::optional<QString>;
    [[nodiscard]] auto isValid() const -> bool override;

  private slots:
    void validateInput();
};

}

#endif