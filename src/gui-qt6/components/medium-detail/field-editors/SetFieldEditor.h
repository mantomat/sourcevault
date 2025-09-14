#ifndef SETFIELDEDITOR_H
#define SETFIELDEDITOR_H

#include "FieldEditor.h"

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <functional>
#include <optional>
#include <set>

namespace Gui::Components {

class SetFieldEditor : public FieldEditor {
    Q_OBJECT

    QLineEdit *editInput;

    std::function<bool(const QString &)> itemValidator;

  public:
    ~SetFieldEditor() override = default;

    SetFieldEditor(const std::function<bool(const QString &)> &newItemValidator, QWidget *parent);

    SetFieldEditor(const SetFieldEditor &) = delete;
    SetFieldEditor(SetFieldEditor &&) = delete;
    auto operator=(const SetFieldEditor &) -> SetFieldEditor & = delete;
    auto operator=(SetFieldEditor &&) -> SetFieldEditor & = delete;

    void setItems(const std::optional<std::set<QString>> &items);
    [[nodiscard]] auto items() const -> std::optional<std::set<QString>>;

    [[nodiscard]] auto isValid() const -> bool override;

  private:
    [[nodiscard]] auto lineEditToSet() const -> std::set<QString>;

  private slots:
    void validateInput();
};

}

#endif