#include "MenubarController.h"

#include "shared/OverloadedVariantVisitor.h"

#include <QApplication>
#include <type_traits>

using Core::Shared::OverloadedVariantVisitor;

namespace Gui::Components {

MenubarController::MenubarController(Menubar *newMenubar, const Library *newLibrary,
                                     QObject *parent)
    : QObject{parent}
    , library{newLibrary}
    , menubar{newMenubar} {

    initActionsToDialogsConnections();
}

void MenubarController::onExportLibraryAction() {
    emit requestExportLibraryDialog("Export SourceVault Library", "sourcevault-library.json",
                                    "JSON Files (*.json);;All Files (*)");
}
void MenubarController::onExportLocalThumbnailsAction() {
    emit requestExportThumbnailsDialog("Select Directory");
}
void MenubarController::onImportLibraryOverwriteAction() {
    emit requestImportLibraryOverwriteDialog("Import SourceVault Library", "JSON Files (*.json)");
}
void MenubarController::onImportLibraryMergeAction() {
    emit requestImportLibraryMergeDialog("Import SourceVault Library", "JSON Files (*.json)");
}
void MenubarController::onImportLocalThumbnailsActions() {
    emit requestImportThumbnailsDialog("Select Directory");
}

void MenubarController::onExportLibraryPathChosen(const QString &path) {
    const auto result{jsonPersistenceManager.exportMedia(library->getMedia(), path)};

    if (result.has_value()) {
        emitFsErrorModalRequest("Library Export Failed", result.value());
        return;
    }
    emit requestSetActionFeedback(QString{"Library succesfully exported! (Total: %1 media)"}.arg(
        QString::number(library->mediaCount())));
}

void MenubarController::onExportThumbnailsPathChosen(const QString &path) {
    const auto result{thumbnailsExporter.exportLocalThumbnails(library->getMedia(), path)};

    std::visit(OverloadedVariantVisitor{
                   [this](const QFileDevice::FileError &err) {
                       emitFsErrorModalRequest("Thumbnails Export Failed", err);
                   },
                   [this](const std::unordered_set<QUuid> &exportedIds) {
                       emit requestSetActionFeedback(
                           QString{"Thumbnails succesfully exported! (Total: %1 thumbnails)"}.arg(
                               QString::number(exportedIds.size())));
                   },
               },
               result);
}

void MenubarController::onImportLibraryOverwritePathChosen(const QString &path) {
    const auto result{jsonPersistenceManager.importLibrary(path)};

    std::visit(
        OverloadedVariantVisitor{
            [this](const QFileDevice::FileError &err) {
                emitFsErrorModalRequest("Library Import Failed", err);
            },
            [this](const JsonDeserializationError &err) { emitDeserErrorModalRequest(err); },
            [this](const Library &lib) {
                emit libraryImportOverwriteRequest(lib);
                emit requestSetActionFeedback(
                    QString{"Library succesfully imported! (Overwrite mode, total %1 media)"}.arg(
                        QString::number(lib.mediaCount())));
            },
        },
        result);
}

void MenubarController::onImportLibraryMergePathChosen(const QString &path) {
    const auto result{jsonPersistenceManager.importLibrary(path)};

    std::visit(
        OverloadedVariantVisitor{
            [this](const QFileDevice::FileError &err) {
                emitFsErrorModalRequest("Library Import Failed", err);
            },
            [this](const JsonDeserializationError &err) { emitDeserErrorModalRequest(err); },
            [this](const Library &lib) {
                emit libraryImportMergeRequest(lib);
                emit requestSetActionFeedback(QString{
                    "Library succesfully imported! (Merge mode, total: %1 media before merging)"}
                                                  .arg(QString::number(lib.mediaCount() +
                                                                       library->mediaCount())));
            },
        },
        result);
}

void MenubarController::onImportThumbnailsPathChosen(const QString &path) {
    const auto result{thumbnailsImporter.importLocalThumbnails(*library, path)};

    std::visit(
        OverloadedVariantVisitor{
            [this](const QFileDevice::FileError &err) {
                emitFsErrorModalRequest("Thumbnails Import Failed", err);
            },
            [this](const std::pair<Library, std::unordered_set<QUuid>> &success) {
                emit thumbnailsImportRequest(success.first);

                QString feedback{
                    success.second.empty()
                        ? QString{"No thumbnails to import in that location."}
                        : QString{"Thumbnails succesfully imported! (Total: %1 thumbnails)"}.arg(
                              QString::number(success.second.size()))};
                emit requestSetActionFeedback(feedback);
            },
        },
        result);
}

void MenubarController::initActionsToDialogsConnections() {
    connect(menubar->getExportLibraryAction(), &QAction::triggered, this,
            &MenubarController::onExportLibraryAction);
    connect(menubar->getExportLocalThumbnailsAction(), &QAction::triggered, this,
            &MenubarController::onExportLocalThumbnailsAction);
    connect(menubar->getimportLibraryOverwriteAction(), &QAction::triggered, this,
            &MenubarController::onImportLibraryOverwriteAction);
    connect(menubar->getImportLibraryMergeAction(), &QAction::triggered, this,
            &MenubarController::onImportLibraryMergeAction);
    connect(menubar->getImportLocalThumbnailsAction(), &QAction::triggered, this,
            &MenubarController::onImportLocalThumbnailsActions);
}

void MenubarController::emitFsErrorModalRequest(const QString &title,
                                                QFileDevice::FileError errorCode) {
    const auto errorMessage{QString{"A file system error has occured.\n\nError code: %1"}.arg(
        QString::number(static_cast<std::underlying_type_t<QFileDevice::FileError>>(errorCode)))};
    emit requestErrorModal(title, errorMessage);
}
void MenubarController::emitDeserErrorModalRequest(const JsonDeserializationError &error) {
    const QString title{"Library Import Failed"};
    QString errorInfo{"A deserialization error occurred.\n\n"};
    errorInfo += QString("Error: %1\n").arg(error.message);

    const QByteArray jsonData =
        std::visit(OverloadedVariantVisitor{
                       [](const QJsonDocument &doc) { return doc.toJson(); },
                       [](const QJsonArray &arr) { return QJsonDocument{arr}.toJson(); },
                       [](const QJsonObject &obj) { return QJsonDocument{obj}.toJson(); }},
                   error.errorLocation);

    QFontMetrics metrics(QApplication::font());
    QString elidedJson = metrics.elidedText(jsonData, Qt::ElideRight, 400);

    errorInfo += QString("\nError Location:\n%1").arg(elidedJson);

    emit requestErrorModal(title, errorInfo);
}

}