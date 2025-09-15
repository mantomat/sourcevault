#include "BookDetailPage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Gui::Components {

BookDetailPage::BookDetailPage(const MediumDetailSection::Dependencies &mediumDeps,
                               const UserDataDetailSection::Dependencies &userDataDeps,
                               const BookDetailSection::Dependencies &bookDeps, QWidget *parent)
    : DetailPage{parent}
    , thumbnail{new ThumbnailRenderer{std::nullopt, ":/assets/default-thumbnails/book.png", this}}
    , mediumSection{new MediumDetailSection{mediumDeps, this}}
    , userDataSection{new UserDataDetailSection{userDataDeps, this}}
    , bookSection{new BookDetailSection{bookDeps, this}} {

    auto *dataLayout = new QVBoxLayout;
    dataLayout->setContentsMargins(0, 0, 0, 0);
    dataLayout->addWidget(mediumSection);
    dataLayout->addWidget(bookSection);
    dataLayout->addWidget(userDataSection);
    dataLayout->addStretch();

    auto *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(thumbnail);
    mainLayout->addLayout(dataLayout, 1);

    getContentLayout()->addLayout(mainLayout);

    connect(this, &BookDetailPage::saveRequested, this, &BookDetailPage::refreshThumbnail);

    connect(mediumSection, &MediumDetailSection::stateChanged, this,
            &BookDetailPage::onStateChanged);
    connect(bookSection, &BookDetailSection::stateChanged, this, &BookDetailPage::onStateChanged);
    connect(userDataSection, &UserDataDetailSection::stateChanged, this,
            &BookDetailPage::onStateChanged);
};

auto BookDetailPage::getMediumSection() const -> MediumDetailSection * {
    return mediumSection;
}

auto BookDetailPage::getUserDataSection() const -> UserDataDetailSection * {
    return userDataSection;
}

auto BookDetailPage::getBookSection() const -> BookDetailSection * {
    return bookSection;
}

void BookDetailPage::setEditMode(bool isEditing) {
    DetailPage::setEditMode(isEditing);

    mediumSection->setEditMode(isEditing);
    userDataSection->setEditMode(isEditing);
    bookSection->setEditMode(isEditing);
}

void BookDetailPage::refreshThumbnail() {
    thumbnail->trySetPreferred(bookSection->getState().thumbnailUrl);
}

void BookDetailPage::onStateChanged() {
    setSaveButtonDisabled(!(mediumSection->isEverythingValid() &&
                            bookSection->isEverythingValid() &&
                            userDataSection->isEverythingValid()));
}

}