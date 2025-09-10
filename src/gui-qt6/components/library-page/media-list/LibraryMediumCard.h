#ifndef LIBRARYMEDIUMCARD_H
#define LIBRARYMEDIUMCARD_H

#include <QUrl>
#include <QUuid>
#include <QWidget>

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
};

}

#endif