#ifndef LIBRARYMEDIUMCARD_H
#define LIBRARYMEDIUMCARD_H

#include <QUrl>
#include <QUuid>
#include <QVBoxLayout>
#include <QWidget>
#include <components/thumbnail-renderer/ThumbnailRenderer.h>

namespace Gui::Components {

class LibraryMediumCard : public QWidget {
    Q_OBJECT

  public:
    struct MediumCardViewModel {
        QUuid id;
        QString type;
        QString title;
        std::optional<QString> authors;
        std::optional<QUrl> thumbnailUrl;
        QString fallbackThumbnailQrcPath;
    };

    LibraryMediumCard(const MediumCardViewModel &cardData, QWidget *parent);

  private:
    [[nodiscard]] auto initThumbnailRenderer(const MediumCardViewModel &cardData)
        -> ThumbnailRenderer *;
    [[nodiscard]] auto initTitleLabel(const MediumCardViewModel &cardData) -> QLabel *;
    [[nodiscard]] auto initInfoLabel(const MediumCardViewModel &cardData) -> QLabel *;
    void initLayout(ThumbnailRenderer *thumbnailRenderer, QLabel *titleLabel, QLabel *infoLabel);
};

}

#endif