#include "LibraryMediumCard.h"

#include "components/thumbnail-renderer/ThumbnailRenderer.h"

#include <QWidget>

namespace Gui::Components {

LibraryMediumCard::LibraryMediumCard(const MediumCardViewModel &cardData, QWidget *parent)
    : QWidget{parent} {

    initLayout(initThumbnailRenderer(cardData), initTitleLabel(cardData), initInfoLabel(cardData));
}

auto LibraryMediumCard::initThumbnailRenderer(const MediumCardViewModel &cardData)
    -> ThumbnailRenderer * {
    auto *thumbnailRenderer{
        new ThumbnailRenderer{cardData.thumbnailUrl, cardData.fallbackThumbnailQrcPath, this}};
    thumbnailRenderer->setFixedSize(100, 100);
    return thumbnailRenderer;
}

auto LibraryMediumCard::initTitleLabel(const MediumCardViewModel &cardData) -> QLabel * {
    auto *titleLabel{new QLabel{this}};
    titleLabel->setText(cardData.title);
    titleLabel->setWordWrap(true);

    QFont titleFont{titleLabel->font()};
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    return titleLabel;
}

auto LibraryMediumCard::initInfoLabel(const MediumCardViewModel &cardData) -> QLabel * {
    auto *infoLabel{new QLabel{this}};
    infoLabel->setText(cardData.type + (cardData.authors.has_value()
                                            ? QString{" by "} + cardData.authors.value()
                                            : ""));
    infoLabel->setWordWrap(true);
    return infoLabel;
}

void LibraryMediumCard::initLayout(ThumbnailRenderer *thumbnailRenderer, QLabel *titleLabel,
                                   QLabel *infoLabel) {
    setFixedWidth(100);

    auto *cardLayout{new QVBoxLayout{this}};
    cardLayout->addWidget(thumbnailRenderer);
    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(infoLabel);
}

}