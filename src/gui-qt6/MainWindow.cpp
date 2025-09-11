#include "MainWindow.h"

namespace Gui {

MainWindow::MainWindow()
    : menubar{new Menubar{this}}
    , mainStack{new QStackedWidget{this}}
    , libraryPage{new LibraryPage{this}} {

    setMenuBar(menubar);

    mainStack->addWidget(libraryPage);

    setCentralWidget(mainStack);

    resize(960, 540);
}

auto MainWindow::getLibraryPage() const -> LibraryPage * {
    return libraryPage;
}

auto MainWindow::getMenubar() const -> Menubar * {
    return menubar;
}

}
