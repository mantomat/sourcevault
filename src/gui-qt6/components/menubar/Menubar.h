#ifndef MENUBAR_H
#define MENUBAR_H

#include <QAction>
#include <QMenuBar>

namespace Gui::Components {

class Menubar : public QMenuBar {
    Q_OBJECT

    QAction *exportLibrary;
    QAction *exportLocalThumbnails;

    QAction *importLibraryOverwrite;
    QAction *importLibraryMerge;
    QAction *importLocalThumbnails;

  public:
    Menubar(QWidget *parent);

    [[nodiscard]] auto getExportLibraryAction() const -> QAction *;
    [[nodiscard]] auto getExportLocalThumbnailsAction() const -> QAction *;
    [[nodiscard]] auto getimportLibraryOverwriteAction() const -> QAction *;
    [[nodiscard]] auto getImportLibraryMergeAction() const -> QAction *;
    [[nodiscard]] auto getImportLocalThumbnailsAction() const -> QAction *;

  private:
    void initImportExportActions();
};

}

#endif