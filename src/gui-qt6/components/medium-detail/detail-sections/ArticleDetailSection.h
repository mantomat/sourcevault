#ifndef ARTICLEDETAILSECTION_H
#define ARTICLEDETAILSECTION_H

#include "components/medium-detail/field-editors/DateFieldEditor.h"
#include "components/medium-detail/field-editors/NumberFieldEditor.h"
#include "components/medium-detail/field-editors/StringFieldEditor.h"
#include "components/medium-detail/field-editors/UrlFieldEditor.h"

#include <QDate>
#include <QUrl>
#include <QWidget>

namespace Gui::Components {

class ArticleDetailSection : public QWidget {
    Q_OBJECT

  public:
    struct ArticleDetailViewModel {
        std::optional<QUrl> articleUrl;
        std::optional<QString> sourceName;
        std::optional<unsigned int> readTimeMinutes;
        std::optional<QDate> publicationDate;
    };

    struct Dependencies {
        std::function<bool(const QUrl &)> articleUrlValidator;
        std::function<bool(const QString &)> sourceNameValidator;
        std::function<bool(const unsigned int &)> readTimeMinutesValidator;
        std::function<bool(const QDate &)> publicationDateValidator;
    };

  private:
    UrlFieldEditor *articleUrlEditor;
    StringFieldEditor *sourceNameEditor;
    NumberFieldEditor *readTimeMinutesFieldEditor;
    DateFieldEditor *publicationDateFieldEditor;

  public:
    ~ArticleDetailSection() override = default;

    ArticleDetailSection(const Dependencies &deps, QWidget *parent);

    ArticleDetailSection(const ArticleDetailSection &) = delete;
    ArticleDetailSection(ArticleDetailSection &&) = delete;
    auto operator=(const ArticleDetailSection &) -> ArticleDetailSection & = delete;
    auto operator=(ArticleDetailSection &&) -> ArticleDetailSection & = delete;

    [[nodiscard]] auto isEverythingValid() const -> bool;

  public slots:
    [[nodiscard]] auto getState() const -> ArticleDetailViewModel;
    void setState(const ArticleDetailViewModel &initData);
    void setEditMode(bool isEditing);

  signals:
    void stateChanged();
};

}

#endif