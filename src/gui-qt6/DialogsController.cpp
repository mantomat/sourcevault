#include "DialogsController.h"

#include <QFileDialog>
#include <QMessageBox>

namespace Gui {

DialogsController::DialogsController(MainWindow *newMainWindow, QObject *parent)
    : QObject{parent}
    , mainWindow{newMainWindow} {}

void DialogsController::onRequestExportLibraryDialog(const QString &dialogName,
                                                     const QString &defaultFileName,
                                                     const QString &fileType) {
    QString path{QFileDialog::getSaveFileName(mainWindow, dialogName, defaultFileName, fileType)};
    if (!path.isEmpty()) {
        emit exportLibraryPathChosen(path);
    }
}

void DialogsController::onRequestExportThumbnailsDialog(const QString &dialogName) {
    QString path{QFileDialog::getExistingDirectory(mainWindow, dialogName)};
    if (!path.isEmpty()) {
        emit exportThumbnailsPathChosen(path);
    }
}

void DialogsController::onRequestImportLibraryOverwriteDialog(const QString &dialogName,
                                                              const QString &fileType) {

    QString path{QFileDialog::getOpenFileName(mainWindow, dialogName, "", fileType)};
    if (!path.isEmpty()) {
        emit importLibraryOverwritePathChosen(path);
    }
}

void DialogsController::onRequestImportLibraryMergeDialog(const QString &dialogName,
                                                          const QString &fileType) {
    QString path{QFileDialog::getOpenFileName(mainWindow, dialogName, "", fileType)};
    if (!path.isEmpty()) {
        emit importLibraryMergePathChosen(path);
    }
}

void DialogsController::onRequestImportThumbnailsDialog(const QString &dialogName) {
    QString path{QFileDialog::getExistingDirectory(mainWindow, dialogName)};
    if (!path.isEmpty()) {
        emit importThumbnailsPathChosen(path);
    }
}

void DialogsController::onRequestErrorModal(const QString &errorTitle, const QString &errorInfo) {
    QMessageBox::critical(mainWindow, errorTitle, errorInfo);
}

void DialogsController::onRequestThumbnailUrlDialog() {
    QString path{QFileDialog::getOpenFileName(mainWindow, "Select Thumbnail Image", "",
                                              "Image Files (*.png *.jpg *.jpeg);;All Files (*)")};
    if (!path.isEmpty()) {
        emit thumbnailUrlPathChosen(path);
    }
}

}