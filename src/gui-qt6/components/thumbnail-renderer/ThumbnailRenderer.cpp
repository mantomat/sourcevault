#include "ThumbnailRenderer.h"

#include <QVBoxLayout>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qpixmap.h>

namespace Gui::Components {

ThumbnailRenderer::ThumbnailRenderer(const std::optional<QUrl> &preferredThumbnailUrl,
                                     const QString &fallbackThumbnailQrcPath, QWidget *parent)
    : QWidget{parent}
    , fallbackThumbnailPath{fallbackThumbnailQrcPath}
    , imageLabel{new QLabel{this}}
    , networkManager{new QNetworkAccessManager{this}} {

    auto *layout{new QVBoxLayout(this)};
    layout->addWidget(imageLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);

    imageLabel->setScaledContents(true);

    QPixmap loadingPixmap{":/assets/loading.png"};
    imageLabel->setPixmap(loadingPixmap);

    if (!preferredThumbnailUrl.has_value() || !preferredThumbnailUrl.value().isValid()) {
        setFallback();
        return;
    }

    if (preferredThumbnailUrl.value().isLocalFile()) {
        QPixmap localFilePixmap{preferredThumbnailUrl.value().toLocalFile()};
        if (!localFilePixmap.isNull()) {
            imageLabel->setPixmap(localFilePixmap);
        } else {
            qWarning() << "Error: local image file is invalid"
                       << preferredThumbnailUrl.value().toLocalFile();
            setFallback();
        }
        return;
    }

    networkManager->get(QNetworkRequest(preferredThumbnailUrl.value()));
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