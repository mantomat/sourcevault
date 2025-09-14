#ifndef DATEFIELDEDITOR_H
#define DATEFIELDEDITOR_H

#include "FieldEditor.h"

#include <QCheckBox>
#include <QDateEdit>

namespace Gui::Components {

class DateFieldEditor : public FieldEditor {
    Q_OBJECT

    QDateEdit *editInput;

    std::function<bool(const QDate &)> validator;

    static constexpr const char *dateFormat{"yyyy-MM-dd"};

  public:
    ~DateFieldEditor() override = default;

    DateFieldEditor(const std::function<bool(const QDate &)> &newValidator, QWidget *parent);

    DateFieldEditor(const DateFieldEditor &) = delete;
    DateFieldEditor(DateFieldEditor &&) = delete;
    auto operator=(const DateFieldEditor &) -> DateFieldEditor & = delete;
    auto operator=(DateFieldEditor &&) -> DateFieldEditor & = delete;

    void setDate(const std::optional<QDate> &date);
    [[nodiscard]] auto date() const -> std::optional<QDate>;
    [[nodiscard]] auto isValid() const -> bool override;

  private slots:
    void validateInput();
};
}

#endif