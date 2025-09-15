#ifndef BOOKDETAILSECTION_H
#define BOOKDETAILSECTION_H

#include "components/medium-detail/field-editors/NumberFieldEditor.h"
#include "components/medium-detail/field-editors/StringFieldEditor.h"
#include "components/medium-detail/field-editors/TextFieldEditor.h"
#include "components/medium-detail/field-editors/UrlFieldEditor.h"

#include <QUrl>
#include <QWidget>

namespace Gui::Components {

class BookDetailSection : public QWidget {
    Q_OBJECT

  public:
    struct BookDetailViewModel {
        std::optional<QString> isbn;
        std::optional<QString> edition;
        std::optional<QString> publisher;
        std::optional<int> yearPublished;
        std::optional<unsigned int> pageNumber;
        std::optional<QString> description;
        std::optional<QUrl> thumbnailUrl;
    };

    struct Dependencies {
        std::function<bool(const QString &)> isbnValidator;
        std::function<bool(const QString &)> editionValidator;
        std::function<bool(const QString &)> publisherValidator;
        std::function<bool(const int &)> yearPublishedValidator;
        std::function<bool(const unsigned int &)> pageNumberValidator;
        std::function<bool(const QString &)> descriptionValidator;
        std::function<bool(const QUrl &)> thumbnailUrlValidator;
    };

  private:
    StringFieldEditor *isbnEditor;
    StringFieldEditor *editionEditor;
    StringFieldEditor *publisherEditor;
    NumberFieldEditor *yearPublishedEditor;
    NumberFieldEditor *pageNumberEditor;
    TextFieldEditor *descriptionEditor;
    UrlFieldEditor *thumbnailUrlEditor;

  public:
    ~BookDetailSection() override = default;

    BookDetailSection(const Dependencies &deps, QWidget *parent);

    BookDetailSection(const BookDetailSection &) = delete;
    BookDetailSection(BookDetailSection &&) = delete;
    auto operator=(const BookDetailSection &) -> BookDetailSection & = delete;
    auto operator=(BookDetailSection &&) -> BookDetailSection & = delete;

    [[nodiscard]] auto isEverythingValid() const -> bool;

  public slots:
    [[nodiscard]] auto getState() const -> BookDetailViewModel;
    void setState(const BookDetailViewModel &initData);
    void setEditMode(bool isEditing);

    void onThumbnailPathSelected(const QString &path);

  signals:
    void stateChanged();
    void thumbnailPathDialogRequested();
};

}

#endif