#ifndef USERDATADETAILSECTION_H
#define USERDATADETAILSECTION_H

#include "components/medium-detail/field-editors/BooleanFieldEditor.h"
#include "components/medium-detail/field-editors/PriorityFieldEditor.h"
#include "components/medium-detail/field-editors/SetFieldEditor.h"
#include "components/medium-detail/field-editors/TextFieldEditor.h"

#include <QWidget>
#include <functional>
#include <optional>
#include <set>

namespace Gui::Components {

class UserDataDetailSection : public QWidget {
    Q_OBJECT

  public:
    struct UserDataDetailViewModel {
        bool favorite{false};
        std::optional<std::set<QString>> topics;
        std::optional<QString> notes;
        std::optional<PriorityFieldEditor::Priority> priority;
    };

    struct Dependencies {
        std::function<bool(const QString &)> notesValidator;
        std::function<bool(const QString &)> topicValidator;
    };

  private:
    BooleanFieldEditor *favoriteEditor;
    SetFieldEditor *topicsEditor;
    TextFieldEditor *notesEditor;
    PriorityFieldEditor *priorityEditor;

  public:
    ~UserDataDetailSection() override = default;

    UserDataDetailSection(const Dependencies &deps, QWidget *parent);

    UserDataDetailSection(const UserDataDetailSection &) = delete;
    UserDataDetailSection(UserDataDetailSection &&) = delete;
    auto operator=(const UserDataDetailSection &) -> UserDataDetailSection & = delete;
    auto operator=(UserDataDetailSection &&) -> UserDataDetailSection & = delete;

    [[nodiscard]] auto isEverythingValid() const -> bool;

  public slots:
    [[nodiscard]] auto getState() const -> UserDataDetailViewModel;
    void setState(const UserDataDetailViewModel &initData);
    void setEditMode(bool isEditing);

  signals:
    void stateChanged();
};

}

#endif