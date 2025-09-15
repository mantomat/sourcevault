#include "LibraryMediumCard.h"

#include "components/thumbnail-renderer/ThumbnailRenderer.h"

#include <QFontMetrics>
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
    thumbnailRenderer->setFixedSize(width, 100);
    return thumbnailRenderer;
}

auto LibraryMediumCard::initTitleLabel(const MediumCardViewModel &cardData) -> QLabel * {
    auto *titleLabel{new QLabel{this}};

    QFontMetrics metrics(titleLabel->font());
    QString elidedText =
        metrics.elidedText(cardData.title, Qt::ElideRight, width - (margins * 2) - 5);
    titleLabel->setText(elidedText);

    QFont titleFont{titleLabel->font()};
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    return titleLabel;
}

auto LibraryMediumCard::initInfoLabel(const MediumCardViewModel &cardData) -> QLabel * {
    auto *infoLabel{new QLabel{this}};
    QFontMetrics metrics(infoLabel->font());

    QString infoText{cardData.type + (cardData.authors.has_value()
                                          ? QString{" by "} + cardData.authors.value()
                                          : "")};
    QString elidedText = metrics.elidedText(infoText, Qt::ElideRight, width - (margins * 2) - 5);

    infoLabel->setText(elidedText);
    infoLabel->setWordWrap(true);

    return infoLabel;
}

void LibraryMediumCard::initLayout(ThumbnailRenderer *thumbnailRenderer, QLabel *titleLabel,
                                   QLabel *infoLabel) {
    setFixedWidth(width);
    setCursor(Qt::PointingHandCursor);

    auto *cardLayout{new QVBoxLayout{this}};
    cardLayout->setContentsMargins(margins, margins, margins, margins);
    cardLayout->addWidget(thumbnailRenderer);
    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(infoLabel);
}

}