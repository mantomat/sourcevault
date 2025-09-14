#ifndef PRIORITYFIELDEDITOR_H
#define PRIORITYFIELDEDITOR_H

#include "FieldEditor.h"

#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
#include <QWidget>

namespace Gui::Components {

class PriorityFieldEditor : public FieldEditor {
    Q_OBJECT

  public:
    enum class Priority : std::uint8_t { Min, Low, Mid, High, Max };

  private:
    QComboBox *editInput;

  public:
    ~PriorityFieldEditor() override = default;

    PriorityFieldEditor(QWidget *parent);

    PriorityFieldEditor(const PriorityFieldEditor &) = delete;
    PriorityFieldEditor(PriorityFieldEditor &&) = delete;
    auto operator=(const PriorityFieldEditor &) -> PriorityFieldEditor & = delete;
    auto operator=(PriorityFieldEditor &&) -> PriorityFieldEditor & = delete;

    void setPriority(std::optional<Priority> priority);
    [[nodiscard]] auto priority() const -> std::optional<Priority>;
    [[nodiscard]] auto isValid() const -> bool override;

  private:
    void initEditor();
};

}

#endif