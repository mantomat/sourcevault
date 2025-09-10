#ifndef THUMBNAILRENDERER_H
#define THUMBNAILRENDERER_H

#include <QLabel>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QWidget>

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
    void setFallback();

  private slots:
    void onNetworkReply(QNetworkReply *reply);
};

}

#endif