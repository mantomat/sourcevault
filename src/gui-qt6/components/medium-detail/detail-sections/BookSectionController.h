#ifndef BOOKSECTIONCONTROLLER_H
#define BOOKSECTIONCONTROLLER_H

#include "BookDetailSection.h"
#include "model/Book.h"

#include <QObject>

using Core::Model::Book;

namespace Gui::Components {

class BookSectionController : public QObject {
    Q_OBJECT

    BookDetailSection *section;

  public:
    ~BookSectionController() override = default;

    BookSectionController(BookDetailSection *newSection, const Book *bookToDisplay,
                          QObject *parent);

    BookSectionController(const BookSectionController &) = delete;
    BookSectionController(BookSectionController &&) = delete;
    auto operator=(const BookSectionController &) -> BookSectionController & = delete;
    auto operator=(BookSectionController &&) -> BookSectionController & = delete;

    void applyChanges(Book &bookToModify) const;

  private:
    void populateSection(const Book *bookToDisplay);
};

}

#endif