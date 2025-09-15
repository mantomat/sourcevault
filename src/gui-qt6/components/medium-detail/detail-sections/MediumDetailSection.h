#ifndef MEDIUMDETAILSECTION_H
#define MEDIUMDETAILSECTION_H

#include "components/medium-detail/field-editors/SetFieldEditor.h"
#include "components/medium-detail/field-editors/StringFieldEditor.h"

#include <QDate>
#include <QString>
#include <QUuid>
#include <QWidget>
#include <set>

namespace Gui::Components {

class MediumDetailSection : public QWidget {
    Q_OBJECT

  public:
    struct MediumDetailViewModel {
        QString title;
        QDate dateAdded;
        QUuid id;
        std::optional<std::set<QString>> authors;
        std::optional<QString> language;
    };

    struct Dependencies {
        std::function<bool(const QString &)> titleValidator;
        std::function<bool(const QString &)> authorValidator;
        std::function<bool(const QString &)> languageValidator;
    };

  private:
    StringFieldEditor *titleEditor;
    SetFieldEditor *authorsEditor;
    StringFieldEditor *languageEditor;

    QLabel *dateAddedLabel;
    QLabel *idLabel;

  public:
    ~MediumDetailSection() override = default;

    MediumDetailSection(const Dependencies &deps, QWidget *parent);

    MediumDetailSection(const MediumDetailSection &) = delete;
    MediumDetailSection(MediumDetailSection &&) = delete;
    auto operator=(const MediumDetailSection &) -> MediumDetailSection & = delete;
    auto operator=(MediumDetailSection &&) -> MediumDetailSection & = delete;

    [[nodiscard]] auto isEverythingValid() const -> bool;

  public slots:
    [[nodiscard]] auto getState() const -> MediumDetailViewModel;
    void setState(const MediumDetailViewModel &initData);
    void setEditMode(bool isEditing);

  signals:
    void stateChanged();
};

}

#endif