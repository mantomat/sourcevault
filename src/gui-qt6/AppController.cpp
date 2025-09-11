#include "AppController.h"

#include "DialogsController.h"
#include "components/menubar/MenubarController.h"
#include "model/MediumUserData.h"

using Core::Model::MediumUserData;

namespace Gui {

AppController::AppController(MainWindow *newMainWindow)
    : library{std::make_unique<Library>()}
    , mainWindow{newMainWindow}
    , libraryPageController{new LibraryPageController{mainWindow->getLibraryPage(), library.get(),
                                                      this}}
    , menubarController{new MenubarController{mainWindow->getMenubar(), library.get(), this}}
    , dialogsController{new DialogsController{mainWindow, this}} {

    initMenubarToDialogsConnections();
    initDialogsToMenubarConnections();
    initMenubarToThisConnections();
}

void AppController::onLibraryImportOverwriteRequest(const Library &lib) {
    *library = lib;
    libraryPageController->refreshMediaList();
}
void AppController::onLibraryImportMergeRequest(const Library &lib) {
    library->merge(lib);
    libraryPageController->refreshMediaList();
}
void AppController::onThumbnailsImportRequest(const Library &lib) {
    library = std::make_unique<Library>(lib);
    libraryPageController->refreshMediaList();
}

void AppController::initMenubarToDialogsConnections() {
    connect(menubarController, &MenubarController::requestExportLibraryDialog, dialogsController,
            &DialogsController::onRequestExportLibraryDialog);
    connect(menubarController, &MenubarController::requestExportThumbnailsDialog, dialogsController,
            &DialogsController::onRequestExportThumbnailsDialog);
    connect(menubarController, &MenubarController::requestImportLibraryOverwriteDialog,
            dialogsController, &DialogsController::onRequestImportLibraryOverwriteDialog);
    connect(menubarController, &MenubarController::requestImportLibraryMergeDialog,
            dialogsController, &DialogsController::onRequestImportLibraryMergeDialog);
    connect(menubarController, &MenubarController::requestImportThumbnailsDialog, dialogsController,
            &DialogsController::onRequestImportThumbnailsDialog);

    connect(menubarController, &MenubarController::requestErrorModal, dialogsController,
            &DialogsController::onRequestErrorModal);
}

void AppController::initDialogsToMenubarConnections() {
    connect(dialogsController, &DialogsController::exportLibraryPathChosen, menubarController,
            &MenubarController::onExportLibraryPathChosen);
    connect(dialogsController, &DialogsController::exportThumbnailsPathChosen, menubarController,
            &MenubarController::onExportThumbnailsPathChosen);
    connect(dialogsController, &DialogsController::importLibraryOverwritePathChosen,
            menubarController, &MenubarController::onImportLibraryOverwritePathChosen);
    connect(dialogsController, &DialogsController::importLibraryMergePathChosen, menubarController,
            &MenubarController::onImportLibraryMergePathChosen);
    connect(dialogsController, &DialogsController::importThumbnailsPathChosen, menubarController,
            &MenubarController::onImportThumbnailsPathChosen);
}

void AppController::initMenubarToThisConnections() {
    connect(menubarController, &MenubarController::libraryImportOverwriteRequest, this,
            &AppController::onLibraryImportOverwriteRequest);
    connect(menubarController, &MenubarController::libraryImportMergeRequest, this,
            &AppController::onLibraryImportMergeRequest);
    connect(menubarController, &MenubarController::thumbnailsImportRequest, this,
            &AppController::onThumbnailsImportRequest);
}

}
