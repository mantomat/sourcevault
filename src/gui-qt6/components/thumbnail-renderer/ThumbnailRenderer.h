#ifndef THUMBNAILRENDERER_H
#define THUMBNAILRENDERER_H

#include <QLabel>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QWidget>
#include <qboxlayout.h>

namespace Gui::Components {

class ThumbnailRenderer : public QWidget {
    Q_OBJECT

    QString fallbackThumbnailPath;
    QLabel *imageLabel;
    QNetworkAccessManager *networkManager;

  public:
    ThumbnailRenderer(const std::optional<QUrl> &preferredThumbnailUrl,
                      const QString &fallbackThumbnailQrcPath, QWidget *parent);

  private:
    void initLayout();
    void initImageLabel();
    void trySetImageLabel(const std::optional<QUrl> &preferredThumbnailUrl);
    void trySetPreferred(const QUrl &preferredThumbnailUrl);
    void trySetLocalPreferred(const QUrl &preferredThumbnailUrl);
    void trySetOnlinePreferred(const QUrl &preferredThumbnailUrl);
    void setFallback();

  private slots:
    void onNetworkReply(QNetworkReply *reply);
};

}

#endif