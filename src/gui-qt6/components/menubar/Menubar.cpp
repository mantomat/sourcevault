#include "Menubar.h"

namespace Gui::Components {

Menubar::Menubar(QWidget *parent)
    : QMenuBar{parent}
    , exportLibrary{new QAction{"Export &Library", this}}
    , exportLocalThumbnails{new QAction{"Export Local &Thumbnails", this}}
    , importLibraryOverwrite{new QAction{"Import Library (&overwrite)", this}}
    , importLibraryMerge{new QAction{"Import Library (&merge)", this}}
    , importLocalThumbnails{new QAction{"Import Local &Thumbnails", this}} {

    initImportExportActions();
}

auto Menubar::getExportLibraryAction() const -> QAction * {
    return exportLibrary;
}
auto Menubar::getExportLocalThumbnailsAction() const -> QAction * {
    return exportLocalThumbnails;
}
auto Menubar::getimportLibraryOverwriteAction() const -> QAction * {
    return importLibraryOverwrite;
}
auto Menubar::getImportLibraryMergeAction() const -> QAction * {
    return importLibraryMerge;
}
auto Menubar::getImportLocalThumbnailsAction() const -> QAction * {
    return importLocalThumbnails;
}

void Menubar::initImportExportActions() {
    auto *fileMenu{addMenu("&File")};

    auto *exportMenu{fileMenu->addMenu("&Export")};
    exportMenu->addAction(exportLibrary);
    exportMenu->addAction(exportLocalThumbnails);

    auto *importMenu{fileMenu->addMenu("&Import")};
    importMenu->addAction(importLibraryOverwrite);
    importMenu->addAction(importLibraryMerge);
    importMenu->addAction(importLocalThumbnails);
}

}