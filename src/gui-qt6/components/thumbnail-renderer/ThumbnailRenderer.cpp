#include "ThumbnailRenderer.h"

#include <QNetworkAccessManager>
#include <QVBoxLayout>
#include <qnetworkreply.h>
#include <qpixmap.h>

namespace Gui::Components {

ThumbnailRenderer::ThumbnailRenderer(const std::optional<QUrl> &preferredThumbnailUrl,
                                     const QString &fallbackThumbnailQrcPath, QWidget *parent)
    : QWidget{parent}
    , fallbackThumbnailPath{fallbackThumbnailQrcPath}
    , imageLabel{new QLabel{this}}
    , networkManager{new QNetworkAccessManager{this}}
    , currentPixmap{} {

    initLayout();
    initImageLabel();

    if (preferredThumbnailUrl.has_value()) {
        trySetPreferred(preferredThumbnailUrl.value());
    } else {
        setFallback();
    }
}

void ThumbnailRenderer::initLayout() {
    auto *layout{new QVBoxLayout{this}};

    layout->addWidget(imageLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
}

void ThumbnailRenderer::initImageLabel() {
    setCustomPixmap(QPixmap(":/assets/loading.png"));
}

void ThumbnailRenderer::trySetPreferred(const QUrl &preferredThumbnailUrl) {
    if (preferredThumbnailUrl.isLocalFile()) {
        trySetLocalPreferred(preferredThumbnailUrl);
    } else {
        trySetOnlinePreferred(preferredThumbnailUrl);
    }
}

void ThumbnailRenderer::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (!currentPixmap.isNull()) {
        setCustomPixmap(currentPixmap);
    }
}

void ThumbnailRenderer::trySetLocalPreferred(const QUrl &preferredThumbnailUrl) {
    const QString localFilePath{preferredThumbnailUrl.toLocalFile()};

    auto localFilePixmap{localFilePath};
    if (!localFilePixmap.isNull()) {
        setCustomPixmap(localFilePixmap);
    } else {
        qWarning() << "Error: local image file is invalid" << localFilePath;
        setFallback();
    }
}

void ThumbnailRenderer::trySetOnlinePreferred(const QUrl &preferredThumbnailUrl) {
    networkManager->get(QNetworkRequest(preferredThumbnailUrl));
    connect(networkManager, &QNetworkAccessManager::finished, this,
            &ThumbnailRenderer::onNetworkReply);
}

void ThumbnailRenderer::setFallback() {
    QPixmap fallbackPixmap{fallbackThumbnailPath};
    assert(!fallbackPixmap.isNull());

    setCustomPixmap(fallbackPixmap);
}

void ThumbnailRenderer::setCustomPixmap(QPixmap pixmap) {
    currentPixmap = std::move(pixmap);
    if (!currentPixmap.isNull()) {
        imageLabel->setPixmap(
            currentPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void ThumbnailRenderer::onNetworkReply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Error downloading image:" << reply->errorString();
        setFallback();
        reply->deleteLater();
        return;
    }

    QPixmap pixmap{};
    if (!pixmap.loadFromData(reply->readAll())) {
        qWarning() << "Error: Downloaded data is not a valid image.";
        setFallback();
        reply->deleteLater();
        return;
    }

    setCustomPixmap(pixmap);
    reply->deleteLater();
}
}