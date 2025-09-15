#ifndef DIALOGSCONTROLLER_H
#define DIALOGSCONTROLLER_H

#include "MainWindow.h"

#include <QObject>

namespace Gui {

class DialogsController : public QObject {
    Q_OBJECT

    MainWindow *mainWindow;

  public:
    DialogsController(MainWindow *newMainWindow, QObject *parent);

  public slots:
    void onRequestExportLibraryDialog(const QString &dialogName, const QString &defaultFileName,
                                      const QString &fileType);
    void onRequestExportThumbnailsDialog(const QString &dialogName);
    void onRequestImportLibraryOverwriteDialog(const QString &dialogName, const QString &fileType);
    void onRequestImportLibraryMergeDialog(const QString &dialogName, const QString &fileType);
    void onRequestImportThumbnailsDialog(const QString &dialogName);

    void onRequestErrorModal(const QString &errorTitle, const QString &errorInfo);

    void onRequestThumbnailUrlDialog();

  signals:
    void exportLibraryPathChosen(const QString &path);
    void exportThumbnailsPathChosen(const QString &path);
    void importLibraryOverwritePathChosen(const QString &path);
    void importLibraryMergePathChosen(const QString &path);
    void importThumbnailsPathChosen(const QString &path);
    void thumbnailUrlPathChosen(const QString &path);
};

}

#endif