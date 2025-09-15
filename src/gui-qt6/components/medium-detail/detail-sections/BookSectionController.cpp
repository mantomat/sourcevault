#include "BookSectionController.h"

namespace Gui::Components {

BookSectionController::BookSectionController(BookDetailSection *newSection,
                                             const Book *bookToDisplay, QObject *parent)
    : QObject{parent}
    , section{newSection} {
    populateSection(bookToDisplay);
}

void BookSectionController::applyChanges(Book &bookToModify) const {
    assert(section->isEverythingValid());

    const auto formData{section->getState()};

    if (formData.isbn.has_value()) {
        bookToModify.isbn().set(formData.isbn.value());
    } else {
        bookToModify.isbn().unset();
    }

    if (formData.edition.has_value()) {
        bookToModify.edition().set(formData.edition.value());
    } else {
        bookToModify.edition().unset();
    }

    if (formData.publisher.has_value()) {
        bookToModify.publisher().set(formData.publisher.value());
    } else {
        bookToModify.publisher().unset();
    }

    if (formData.yearPublished.has_value()) {
        bookToModify.yearPublished().set(formData.yearPublished.value());
    } else {
        bookToModify.yearPublished().unset();
    }

    if (formData.pageNumber.has_value()) {
        bookToModify.pageNumber().set(formData.pageNumber.value());
    } else {
        bookToModify.pageNumber().unset();
    }

    if (formData.description.has_value()) {
        bookToModify.description().set(formData.description.value());
    } else {
        bookToModify.description().unset();
    }

    if (formData.thumbnailUrl.has_value()) {
        bookToModify.thumbnailUrl().set(formData.thumbnailUrl.value());
    } else {
        bookToModify.thumbnailUrl().unset();
    }
}

void BookSectionController::populateSection(const Book *bookToDisplay) {
    section->setState({.isbn = bookToDisplay->isbn().get(),
                       .edition = bookToDisplay->edition().get(),
                       .publisher = bookToDisplay->publisher().get(),
                       .yearPublished = bookToDisplay->yearPublished().get(),
                       .pageNumber = bookToDisplay->pageNumber().get(),
                       .description = bookToDisplay->description().get(),
                       .thumbnailUrl = bookToDisplay->thumbnailUrl().get()});
}
}