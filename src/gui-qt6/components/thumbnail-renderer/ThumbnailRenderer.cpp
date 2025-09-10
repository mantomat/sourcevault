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
    , networkManager{new QNetworkAccessManager{this}} {

    initLayout();
    initImageLabel();

    trySetImageLabel(preferredThumbnailUrl);
}

void ThumbnailRenderer::initLayout() {
    auto *layout{new QVBoxLayout{this}};

    layout->addWidget(imageLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
}

void ThumbnailRenderer::initImageLabel() {
    imageLabel->setScaledContents(true);

    QPixmap loadingPixmap{":/assets/loading.png"};
    imageLabel->setPixmap(loadingPixmap);
}

void ThumbnailRenderer::trySetImageLabel(const std::optional<QUrl> &preferredThumbnailUrl) {
    if (!preferredThumbnailUrl.has_value() || !preferredThumbnailUrl.value().isValid()) {
        setFallback();
        return;
    }
    trySetPreferred(preferredThumbnailUrl.value());
}

void ThumbnailRenderer::trySetPreferred(const QUrl &preferredThumbnailUrl) {

    if (preferredThumbnailUrl.isLocalFile()) {
        trySetLocalPreferred(preferredThumbnailUrl);
    }
    trySetOnlinePreferred(preferredThumbnailUrl);
}

void ThumbnailRenderer::trySetLocalPreferred(const QUrl &preferredThumbnailUrl) {
    const QString localFilePath{preferredThumbnailUrl.toLocalFile()};

    QPixmap localFilePixmap{localFilePath};
    if (!localFilePixmap.isNull()) {
        imageLabel->setPixmap(localFilePixmap);
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

    imageLabel->setPixmap(fallbackPixmap);
}

void ThumbnailRenderer::onNetworkReply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Error downloading image:" << reply->errorString();
        setFallback();
        reply->deleteLater();
        return;
    }

    QPixmap pixmap;
    if (!pixmap.loadFromData(reply->readAll())) {
        qWarning() << "Error: Downloaded data is not a valid image.";
        setFallback();
        reply->deleteLater();
        return;
    }

    imageLabel->setPixmap(pixmap);
    reply->deleteLater();
}
}