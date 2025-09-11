#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "DialogsController.h"
#include "MainWindow.h"
#include "components/library-page/LibraryPageController.h"
#include "components/menubar/MenubarController.h"
#include "model/Library.h"

#include <QObject>

using Core::Model::Library;
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

  public:
    AppController(MainWindow *newMainWindow);

  private slots:
    void onLibraryImportOverwriteRequest(const Library &lib);
    void onLibraryImportMergeRequest(const Library &lib);
    void onThumbnailsImportRequest(const Library &lib);
    // void onMediumEdit();
    // void onMediumDetailsRequest(QUuid id);
    // void onMediumEditRequest(QUuid id);
    // void onMediumDeleteRequest(QUuid id);

    void onSetActionFeedbackRequest(const QString &feedback);

  private:
    void initMenubarToDialogsConnections();
    void initDialogsToMenubarConnections();
    void initMenubarToThisConnections();
};

}

#endif