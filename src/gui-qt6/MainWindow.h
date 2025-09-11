#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "components/library-page/LibraryPage.h"
#include "components/menubar/Menubar.h"

#include <QMainWindow>
#include <QStackedWidget>

using Gui::Components::LibraryPage;
using Gui::Components::Menubar;

namespace Gui {

class MainWindow : public QMainWindow {
    Q_OBJECT

    Menubar *menubar;

    QStackedWidget *mainStack;
    LibraryPage *libraryPage;

  public:
    MainWindow();

    [[nodiscard]] auto getLibraryPage() const -> LibraryPage *;
    [[nodiscard]] auto getMenubar() const -> Menubar *;
};

}

#endif
