#ifndef FIELDEDITOR_H
#define FIELDEDITOR_H

#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

namespace Gui::Components {

class FieldEditor : public QWidget {
    Q_OBJECT

    QStackedWidget *stack;

    QWidget *editWidget;
    QWidget *editWidgetContainer;
    QLabel *viewLabel;

    QCheckBox *enabledCheckBox;

  public:
    virtual ~FieldEditor() = default;

    explicit FieldEditor(bool optional, QWidget *parent);

    FieldEditor(const FieldEditor &) = delete;
    FieldEditor(FieldEditor &&) = delete;
    auto operator=(const FieldEditor &) -> FieldEditor & = delete;
    auto operator=(FieldEditor &&) -> FieldEditor & = delete;

    [[nodiscard]] auto isEnabled() const -> bool;
    [[nodiscard]] virtual auto isValid() const -> bool = 0;

  protected:
    static constexpr const char *invalidBackground{"#ffbbbb"};
    static constexpr const char *notSet{""};

    void setViewModeText(const QString &text);
    void setEditModeWidget(QWidget *newEditWidget);

  signals:
    void stateChanged();
    void enabledChanged();

  public slots:
    virtual void setEditMode(bool isEditing);
    void setEnabled(bool enabled);
};

}

#endif