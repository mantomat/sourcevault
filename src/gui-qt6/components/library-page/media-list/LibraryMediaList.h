#ifndef LIBRARYMEDIALIST_H
#define LIBRARYMEDIALIST_H

#include "LibraryMediumCard.h"

#include <QListWidget>
#include <QUrl>
#include <QUuid>
#include <QWidget>

namespace Gui::Components {

class LibraryMediaList : public QWidget {
    Q_OBJECT

    QListWidget *mediaList;

  public:
    LibraryMediaList(QWidget *parent);

    void setMedia(const std::vector<LibraryMediumCard::MediumCardViewModel> &media);
    void removeFromList(const QUuid &id);

  private:
    void initLayout();
    void initMediaList();
};

}

#endif