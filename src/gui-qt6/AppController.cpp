#include "AppController.h"

namespace Gui {

AppController::AppController()
    : libraryPageController{new LibraryPageController{mainWindow.getLibraryPage(), this}} {
    mainWindow.show();
}

}