#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "MainWindow.h"
#include "components/library-page/LibraryPageController.h"
#include "model/Library.h"

#include <QObject>

using Core::Model::Library;
using Gui::Components::LibraryPageController;

namespace Gui {

class AppController : public QObject {
    Q_OBJECT

    MainWindow *mainWindow;

    LibraryPageController *libraryPageController;

  public:
    AppController(MainWindow *newMainWindow);
};

}

#endif