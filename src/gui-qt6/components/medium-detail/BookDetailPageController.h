#ifndef BOOKDETAILPAGECONTROLLER_H
#define BOOKDETAILPAGECONTROLLER_H

#include "components/medium-detail/BookDetailPage.h"
#include "components/medium-detail/DetailPageController.h"
#include "components/medium-detail/detail-sections/BookSectionController.h"
#include "components/medium-detail/detail-sections/MediumSectionController.h"
#include "components/medium-detail/detail-sections/UserDataSectionController.h"
#include "model/Book.h"

using Core::Model::Book;

namespace Gui::Components {

class BookDetailPageController : public DetailPageController {
    Q_OBJECT

    BookDetailPage *view;

    const Book *originalBook;
    Book editableBook;

    MediumSectionController mediumSectionController;
    BookSectionController bookSectionController;
    UserDataSectionController userDataSectionController;

  public:
    ~BookDetailPageController() override = default;

    BookDetailPageController(BookDetailPage *newView, const Book *newOriginalBook, QObject *parent);

    BookDetailPageController(const BookDetailPageController &) = delete;
    BookDetailPageController(BookDetailPageController &&) = delete;
    auto operator=(const BookDetailPageController &) -> BookDetailPageController & = delete;
    auto operator=(BookDetailPageController &&) -> BookDetailPageController & = delete;

  private slots:
    void onSave();
    void onEditRequested();
    void onDiscardChanges();
};

}

#endif