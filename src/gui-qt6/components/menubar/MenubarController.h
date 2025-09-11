#ifndef MENUBARCONTROLLER_H
#define MENUBARCONTROLLER_H

#include "Menubar.h"
#include "model/Library.h"
#include "persistence/json/JsonPersistenceManager.h"
#include "persistence/json/deserialization/JsonDeserializationError.h"
#include "persistence/thumbnails/ThumbnailsExporter.h"
#include "persistence/thumbnails/ThumbnailsImporter.h"

using Core::Model::Library;
using Core::Persistence::Json::JsonDeserializationError;
using Core::Persistence::Json::JsonPersistenceManager;
using Core::Persistence::Thumbnails::ThumbnailsExporter;
using Core::Persistence::Thumbnails::ThumbnailsImporter;

namespace Gui::Components {

class MenubarController : public QObject {
    Q_OBJECT

    const Library *library;
    Menubar *menubar;

    JsonPersistenceManager jsonPersistenceManager;
    ThumbnailsExporter thumbnailsExporter;
    ThumbnailsImporter thumbnailsImporter;

  public:
    MenubarController(Menubar *newMenubar, const Library *newLibrary, QObject *parent);

  private slots:
    void onExportLibraryAction();
    void onExportLocalThumbnailsAction();
    void onImportLibraryOverwriteAction();
    void onImportLibraryMergeAction();
    void onImportLocalThumbnailsActions();

  signals:
    void requestExportLibraryDialog(const QString &dialogName, const QString &defaultFileName,
                                    const QString &fileType);
    void requestExportThumbnailsDialog(const QString &dialogName);
    void requestImportLibraryOverwriteDialog(const QString &dialogName, const QString &fileType);
    void requestImportLibraryMergeDialog(const QString &dialogName, const QString &fileType);
    void requestImportThumbnailsDialog(const QString &dialogName);

    void requestErrorModal(const QString &errorTitle, const QString &errorInfo);

    void libraryImportOverwriteRequest(const Library &lib);
    void libraryImportMergeRequest(const Library &lib);
    void thumbnailsImportRequest(const Library &lib);

  public slots:
    void onExportLibraryPathChosen(const QString &path);
    void onExportThumbnailsPathChosen(const QString &path);
    void onImportLibraryOverwritePathChosen(const QString &path);
    void onImportLibraryMergePathChosen(const QString &path);
    void onImportThumbnailsPathChosen(const QString &path);

  private:
    void initActionsToDialogsConnections();
    void emitFsErrorModalRequest(const QString &title, QFileDevice::FileError errorCode);
    void emitDeserErrorModalRequest(const JsonDeserializationError &error);
};

}

#endif