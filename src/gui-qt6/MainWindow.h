#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "components/library-page/LibraryPage.h"

#include <QMainWindow>
#include <QStackedWidget>

using Gui::Components::LibraryPage;

namespace Gui {

class MainWindow : public QMainWindow {
    Q_OBJECT

    QStackedWidget *mainStack;
    LibraryPage *libraryPage;

  public:
    MainWindow();

    [[nodiscard]] auto getLibraryPage() const -> LibraryPage *;
};

}

#endif
