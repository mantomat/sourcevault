#include "BookCreateController.h"

#include "components/medium-detail/CreateController.h"

namespace Gui::Components {

BookCreateController::BookCreateController(BookDetailPage *newView, QObject *parent)
    : CreateController{parent}
    , view{newView}
    , newBook{Book::create("New Book").value()}
    , mediumSectionController{view->getMediumSection(), &newBook, this}
    , bookSectionController{view->getBookSection(), &newBook, this}
    , userDataSectionController{view->getUserDataSection(), &newBook, this} {
    view->setEditMode(true);

    connect(view, &DetailPage::saveRequested, this, &BookCreateController::onSave);
    connect(view, &DetailPage::discardChanges, this, &BookCreateController::creationCancelled);
}

void BookCreateController::onSave() {
    mediumSectionController.applyChanges(newBook);
    bookSectionController.applyChanges(newBook);
    userDataSectionController.applyChanges(newBook);

    emit mediumCreated(newBook);
}

}