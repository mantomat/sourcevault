#ifndef LIBRARYPAGE_H
#define LIBRARYPAGE_H

#include "media-list/LibraryMediaList.h"
#include "sidebar/LibrarySidebar.h"
#include "topbar/LibraryTopbar.h"

#include <QSplitter>
#include <QWidget>

namespace Gui::Components {

class LibraryPage : public QWidget {
    Q_OBJECT

    QSplitter *splitter;

    LibraryTopbar *topbar;
    LibraryMediaList *mediaList;

    LibrarySidebar *sidebar;
    int sidebarWidth{200};

  public:
    explicit LibraryPage(QWidget *parent);

    [[nodiscard]] auto getSidebar() const -> LibrarySidebar *;
    [[nodiscard]] auto getTopbar() const -> LibraryTopbar *;
    [[nodiscard]] auto getMediaList() const -> LibraryMediaList *;

  private slots:
    void onSidebarToggled(bool showSidebar);
    void onSplitterMoved(int pos, int index);
};

}

#endif