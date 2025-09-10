#include "LibraryMediumCard.h"

#include "components/thumbnail-renderer/ThumbnailRenderer.h"

#include <QVBoxLayout>
#include <QWidget>

namespace Gui::Components {

LibraryMediumCard::LibraryMediumCard(const MediumCardViewModel &cardData, QWidget *parent)
    : QWidget{parent} {

    setFixedWidth(100);

    auto *cardLayout{new QVBoxLayout{this}};

    auto *thumbnailRenderer{
        new ThumbnailRenderer{cardData.thumbnailUrl, cardData.fallbackThumbnailQrcPath, this}};
    thumbnailRenderer->setFixedSize(100, 100);

    auto *titleLabel{new QLabel{this}};
    titleLabel->setText(cardData.title);
    titleLabel->setWordWrap(true);
    QFont titleFont{titleLabel->font()};
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    auto *infoLabel{new QLabel{this}};
    infoLabel->setText(cardData.type + (cardData.authors.has_value()
                                            ? QString{" by "} + cardData.authors.value()
                                            : ""));
    infoLabel->setWordWrap(true);

    cardLayout->addWidget(thumbnailRenderer);
    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(infoLabel);
}

}