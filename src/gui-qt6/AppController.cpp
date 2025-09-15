#include "AppController.h"

#include "DetailPageFactory.h"
#include "DialogsController.h"
#include "components/menubar/MenubarController.h"

using Gui::Components::LibraryMediaList;

namespace Gui {

AppController::AppController(MainWindow *newMainWindow)
    : library{std::make_unique<Library>()}
    , mainWindow{newMainWindow}
    , libraryPageController{new LibraryPageController{mainWindow->getLibraryPage(), library.get(),
                                                      this}}
    , menubarController{new MenubarController{mainWindow->getMenubar(), library.get(), this}}
    , dialogsController{new DialogsController{mainWindow, this}}
    , currentDetailPageController{nullptr}
    , currentCreateController{nullptr} {

    initMenubarToDialogsConnections();
    initDialogsToMenubarConnections();
    initMenubarToThisConnections();
    initMediaListToThisConnections();
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
    *library = lib;
    libraryPageController->refreshMediaList();
}

void AppController::onMediumDetailsRequest(const QUuid &id) {
    auto [page, controller]{
        DetailPageFactory::createDetailPage(library->getMedium(id).value(), mainWindow, this)};

    mainWindow->displayDetailPage(page);
    currentDetailPageController = controller;

    connect(controller, &DetailPageController::mediumEdited, this, &AppController::onMediumEdited);
    connect(controller, &DetailPageController::goBackToLibraryRequest, this,
            &AppController::onMediumDetailsClosed);
}

void AppController::onMediumCreateRequest(LibraryTopbar::MediumTypeViewModel type) {
    auto [page, controller]{DetailPageFactory::createNewMediumPage(type, mainWindow, this)};

    mainWindow->displayDetailPage(page);
    currentCreateController = controller;

    connect(controller, &CreateController::mediumCreated, this, &AppController::onMediumCreated);
    connect(controller, &CreateController::creationCancelled, this,
            &AppController::onMediumCreationAborted);
}

void AppController::onMediumDeleteRequest(const QUuid &id) {
    const auto result{library->removeMedium(id)};
    assert(result);

    libraryPageController->refreshMediaList();
}

void AppController::onMediumEdited(const Medium &updatedMedium) {
    assert(currentDetailPageController != nullptr);

    library->replaceMedium(updatedMedium.clone());

    currentDetailPageController->deleteLater();
    currentDetailPageController = nullptr;

    onMediumDetailsRequest(updatedMedium.id());
}

void AppController::onMediumDetailsClosed() {
    assert(currentDetailPageController != nullptr);
    currentDetailPageController->deleteLater();
    currentDetailPageController = nullptr;

    libraryPageController->refreshMediaList();
    mainWindow->closeDetailPage();
}

void AppController::onMediumCreated(const Medium &createdMedium) {
    assert(currentCreateController != nullptr);
    library->addMedium(createdMedium.clone());

    currentCreateController->deleteLater();
    currentCreateController = nullptr;

    onMediumDetailsRequest(createdMedium.id());
}

void AppController::onMediumCreationAborted() {
    assert(currentCreateController != nullptr);
    currentCreateController->deleteLater();
    currentCreateController = nullptr;

    libraryPageController->refreshMediaList();
    mainWindow->closeDetailPage();
}

void AppController::onSetActionFeedbackRequest(const QString &feedback) {
    mainWindow->setActionFeedback(feedback);
};

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

    connect(menubarController, &MenubarController::requestSetActionFeedback, this,
            &AppController::onSetActionFeedbackRequest);
}

void AppController::initMediaListToThisConnections() {
    connect(mainWindow->getLibraryPage()->getMediaList(), &LibraryMediaList::mediumDetailRequest,
            this, &AppController::onMediumDetailsRequest);
    connect(mainWindow->getLibraryPage()->getMediaList(), &LibraryMediaList::mediumDeleteRequest,
            this, &AppController::onMediumDeleteRequest);
    connect(mainWindow->getLibraryPage()->getTopbar(), &LibraryTopbar::createMediumRequest, this,
            &AppController::onMediumCreateRequest);
}

}
