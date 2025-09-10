#include "AppController.h"

namespace Gui {

AppController::AppController(MainWindow *newMainWindow)
    : mainWindow{newMainWindow}
    , libraryPageController{new LibraryPageController{mainWindow->getLibraryPage(), this}} {}
}