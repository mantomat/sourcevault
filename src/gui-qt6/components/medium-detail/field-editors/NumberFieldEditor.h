#ifndef NUMBERFIELDEDITOR_H
#define NUMBERFIELDEDITOR_H

#include "FieldEditor.h"

#include <QLabel>
#include <QSpinBox>
#include <QStackedWidget>
#include <QWidget>
#include <functional>
#include <optional>

namespace Gui::Components {

class NumberFieldEditor : public FieldEditor {
    Q_OBJECT

    QSpinBox *editInput;

    std::function<bool(int)> validator;

  public:
    ~NumberFieldEditor() override = default;

    NumberFieldEditor(const std::function<bool(int)> &newValidator, QWidget *parent = nullptr);

    NumberFieldEditor(const NumberFieldEditor &) = delete;
    NumberFieldEditor(NumberFieldEditor &&) = delete;
    auto operator=(const NumberFieldEditor &) -> NumberFieldEditor & = delete;
    auto operator=(NumberFieldEditor &&) -> NumberFieldEditor & = delete;

    void setNumber(const std::optional<int> &number);
    [[nodiscard]] auto number() const -> std::optional<int>;

    [[nodiscard]] auto isValid() const -> bool override;

  private slots:
    void validateInput();
};

}

#endif