#include "MainWindow.h"

namespace Gui {

MainWindow::MainWindow()
    : mainStack{new QStackedWidget{this}}
    , libraryPage{new LibraryPage{this}} {

    mainStack->addWidget(libraryPage);

    setCentralWidget(mainStack);

    resize(960, 540);
}

auto MainWindow::getLibraryPage() const -> LibraryPage * {
    return libraryPage;
}

}
