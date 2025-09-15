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
    QPixmap currentPixmap;

  public:
    ThumbnailRenderer(const std::optional<QUrl> &preferredThumbnailUrl,
                      const QString &fallbackThumbnailQrcPath, QWidget *parent);

    void trySetPreferred(const QUrl &preferredThumbnailUrl);

  private:
    void initLayout();
    void initImageLabel();
    void trySetLocalPreferred(const QUrl &preferredThumbnailUrl);
    void trySetOnlinePreferred(const QUrl &preferredThumbnailUrl);
    void setFallback();
    void setCustomPixmap(QPixmap pixmap);

  protected:
    void resizeEvent(QResizeEvent *event) override;

  private slots:
    void onNetworkReply(QNetworkReply *reply);
};

}

#endif