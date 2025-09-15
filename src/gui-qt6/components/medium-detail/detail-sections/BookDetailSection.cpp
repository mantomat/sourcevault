#include "BookDetailSection.h"

#include <QFormLayout>

namespace Gui::Components {

BookDetailSection::BookDetailSection(const Dependencies &deps, QWidget *parent)
    : QWidget(parent)
    , isbnEditor{new StringFieldEditor{deps.isbnValidator, true, this}}
    , editionEditor{new StringFieldEditor{deps.editionValidator, true, this}}
    , publisherEditor{new StringFieldEditor{deps.publisherValidator, true, this}}
    , yearPublishedEditor{new NumberFieldEditor{deps.yearPublishedValidator, this}}
    , pageNumberEditor{new NumberFieldEditor{deps.pageNumberValidator, this}}
    , descriptionEditor{new TextFieldEditor{deps.descriptionValidator, this}}
    , thumbnailUrlEditor{new UrlFieldEditor{deps.thumbnailUrlValidator, true, this}} {

    auto *layout{new QFormLayout{this}};
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addRow("ISBN:", isbnEditor);
    layout->addRow("Edition:", editionEditor);
    layout->addRow("Publisher:", publisherEditor);
    layout->addRow("Year published:", yearPublishedEditor);
    layout->addRow("Page number:", pageNumberEditor);
    layout->addRow("Description:", descriptionEditor);
    layout->addRow("Thumbnail URL:", thumbnailUrlEditor);

    connect(isbnEditor, &StringFieldEditor::stateChanged, this, &BookDetailSection::stateChanged);
    connect(editionEditor, &StringFieldEditor::stateChanged, this,
            &BookDetailSection::stateChanged);
    connect(publisherEditor, &StringFieldEditor::stateChanged, this,
            &BookDetailSection::stateChanged);
    connect(yearPublishedEditor, &NumberFieldEditor::stateChanged, this,
            &BookDetailSection::stateChanged);
    connect(pageNumberEditor, &NumberFieldEditor::stateChanged, this,
            &BookDetailSection::stateChanged);
    connect(descriptionEditor, &TextFieldEditor::stateChanged, this,
            &BookDetailSection::stateChanged);
    connect(thumbnailUrlEditor, &UrlFieldEditor::stateChanged, this,
            &BookDetailSection::stateChanged);
}

auto BookDetailSection::isEverythingValid() const -> bool {
    return isbnEditor->isValid() && editionEditor->isValid() && publisherEditor->isValid() &&
           yearPublishedEditor->isValid() && pageNumberEditor->isValid() &&
           descriptionEditor->isValid() && thumbnailUrlEditor->isValid();
}

auto BookDetailSection::getState() const -> BookDetailViewModel {
    return BookDetailViewModel{
        .isbn = isbnEditor->text(),
        .edition = editionEditor->text(),
        .publisher = publisherEditor->text(),
        .yearPublished = yearPublishedEditor->number(),
        .pageNumber = static_cast<std::optional<unsigned int>>(pageNumberEditor->number()),
        .description = descriptionEditor->text(),
        .thumbnailUrl = thumbnailUrlEditor->url()};
}

void BookDetailSection::setState(const BookDetailViewModel &initData) {
    isbnEditor->setText(initData.isbn);
    editionEditor->setText(initData.edition);
    publisherEditor->setText(initData.publisher);
    yearPublishedEditor->setNumber(initData.yearPublished);
    pageNumberEditor->setNumber(static_cast<std::optional<int>>(initData.pageNumber));
    descriptionEditor->setText(initData.description);
    thumbnailUrlEditor->setUrl(initData.thumbnailUrl);
}

void BookDetailSection::setEditMode(bool isEditing) {
    isbnEditor->setEditMode(isEditing);
    editionEditor->setEditMode(isEditing);
    publisherEditor->setEditMode(isEditing);
    yearPublishedEditor->setEditMode(isEditing);
    pageNumberEditor->setEditMode(isEditing);
    descriptionEditor->setEditMode(isEditing);
    thumbnailUrlEditor->setEditMode(isEditing);
}

}