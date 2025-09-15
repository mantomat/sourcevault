#ifndef BOOKCREATECONTROLLER_H
#define BOOKCREATECONTROLLER_H

#include "components/medium-detail/BookDetailPage.h"
#include "components/medium-detail/CreateController.h"
#include "components/medium-detail/detail-sections/BookSectionController.h"
#include "components/medium-detail/detail-sections/MediumSectionController.h"
#include "components/medium-detail/detail-sections/UserDataSectionController.h"
#include "model/Book.h"

using Core::Model::Book;

namespace Gui::Components {

class BookCreateController : public CreateController {

    BookDetailPage *view;

    Book newBook;

    MediumSectionController mediumSectionController;
    BookSectionController bookSectionController;
    UserDataSectionController userDataSectionController;

  public:
    ~BookCreateController() override = default;

    BookCreateController(BookDetailPage *newView, QObject *parent);

    BookCreateController(const BookCreateController &) = delete;
    BookCreateController(BookCreateController &&) = delete;
    auto operator=(const BookCreateController &) -> BookCreateController & = delete;
    auto operator=(BookCreateController &&) -> BookCreateController & = delete;

  private slots:
    void onSave();
};

}

#endif