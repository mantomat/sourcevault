#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "DetailPageFactory.h"
#include "DialogsController.h"
#include "MainWindow.h"
#include "components/library-page/LibraryPageController.h"
#include "components/medium-detail/DetailPageController.h"
#include "components/menubar/MenubarController.h"
#include "model/Library.h"

#include <QObject>

using Core::Model::Library;
using Gui::Components::DetailPageController;
using Gui::Components::LibraryPageController;
using Gui::Components::MenubarController;

namespace Gui {

/**
 * The main App Controller owns the loaded library and modifies it. Every sub-controller has
 * read-only access of the Library through a pointer.
 */
class AppController : public QObject {
    Q_OBJECT

    std::unique_ptr<Library> library;

    MainWindow *mainWindow;

    LibraryPageController *libraryPageController;
    MenubarController *menubarController;
    DialogsController *dialogsController;

    DetailPageController *currentDetailPageController;
    CreateController *currentCreateController;

  public:
    AppController(MainWindow *newMainWindow);

  private slots:
    void onLibraryImportOverwriteRequest(const Library &lib);
    void onLibraryImportMergeRequest(const Library &lib);
    void onThumbnailsImportRequest(const Library &lib);

    void onMediumDetailsRequest(const QUuid &id);
    void onMediumCreateRequest(LibraryTopbar::MediumTypeViewModel type);
    void onMediumDeleteRequest(const QUuid &id);

    void onMediumEdited(const Medium &updatedMedium);
    void onMediumDetailsClosed();

    void onMediumCreated(const Medium &createdMedium);
    void onMediumCreationAborted();

    void onSetActionFeedbackRequest(const QString &feedback);

  private:
    void initMenubarToDialogsConnections();
    void initDialogsToMenubarConnections();
    void initMenubarToThisConnections();
    void initMediaListToThisConnections();
};

}

#endif