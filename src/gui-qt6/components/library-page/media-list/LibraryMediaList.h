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

  signals:
    void mediumDetailRequest(const QUuid &id);
    void mediumDeleteRequest(const QUuid &id);

  private slots:
    void onItemClicked(QListWidgetItem *item);
    void onContextMenuRequested(const QPoint &pos);

  private:
    void initLayout();
    void initMediaList();
};

}

#endif