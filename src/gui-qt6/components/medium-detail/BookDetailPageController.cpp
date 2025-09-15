#include "BookDetailPageController.h"

namespace Gui::Components {

BookDetailPageController::BookDetailPageController(BookDetailPage *newView,
                                                   const Book *newOriginalBook, QObject *parent)
    : DetailPageController(parent)
    , view{newView}
    , originalBook{newOriginalBook}
    , editableBook{*newOriginalBook}
    , mediumSectionController{view->getMediumSection(), originalBook, this}
    , bookSectionController{view->getBookSection(), originalBook, this}
    , userDataSectionController{view->getUserDataSection(), originalBook, this} {

    view->setEditMode(false);

    connect(view, &DetailPage::saveRequested, this, &BookDetailPageController::onSave);
    connect(view, &DetailPage::editRequested, this, &BookDetailPageController::onEditRequested);
    connect(view, &DetailPage::discardChanges, this, &BookDetailPageController::onDiscardChanges);
    connect(view, &DetailPage::goBackToLibrary, this,
            &BookDetailPageController::goBackToLibraryRequest);
}

void BookDetailPageController::onSave() {
    mediumSectionController.applyChanges(editableBook);
    bookSectionController.applyChanges(editableBook);
    userDataSectionController.applyChanges(editableBook);

    emit mediumEdited(editableBook);
}

void BookDetailPageController::onEditRequested() {
    view->setEditMode(true);
}

void BookDetailPageController::onDiscardChanges() {
    view->setEditMode(false);
}

}