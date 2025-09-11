#include "LibraryPageController.h"

#include "LibraryQueryBuilder.h"
#include "MediumCardsViewModelFactory.h"

namespace Gui::Components {

LibraryPageController::LibraryPageController(LibraryPage *newLibraryPage, const Library *newLibrary,
                                             QObject *parent)
    : QObject{parent}
    , library{newLibrary}
    , libraryPage{newLibraryPage} {

    refreshMediaList();

    connect(libraryPage->getTopbar(), &LibraryTopbar::searchQueryChanged, this,
            &LibraryPageController::onQueryChanged);
    connect(libraryPage->getSidebar(), &LibrarySidebar::stateChanged, this,
            &LibraryPageController::onQueryChanged);
}

void LibraryPageController::refreshMediaList() {
    setMediaCards(queryLibrary(libraryPage->getSidebar()->getState(),
                               libraryPage->getTopbar()->getSearchQuery()));
    libraryPage->getSidebar()->setAvailableTopics(library->getTopicsUnion());
}

void LibraryPageController::setMediaCards(const std::vector<const Medium *> &media) const {
    libraryPage->getMediaList()->setMedia(MediumCardsViewModelFactory::createMediumCards(media));
}

auto LibraryPageController::queryLibrary(const LibrarySidebar::SidebarState &state,
                                         const QString &searchQuery) const
    -> std::vector<const Medium *> {
    std::unique_ptr query{LibraryQueryBuilder::build(state, searchQuery)};
    return query->query(library->getMedia());
}

void LibraryPageController::onQueryChanged() {
    LibrarySidebar *sidebar = libraryPage->getSidebar();
    LibraryTopbar *topbar = libraryPage->getTopbar();

    QString searchQuery = topbar->getSearchQuery();
    LibrarySidebar::SidebarState sidebarState = sidebar->getState();

    sidebar->blockSignals(true);
    if (!searchQuery.isEmpty() && sidebarState.sort != LibrarySidebar::SortTypeOptions::Disabled) {
        sidebar->disableSorting();
    } else if (searchQuery.isEmpty() &&
               sidebarState.sort == LibrarySidebar::SortTypeOptions::Disabled) {
        sidebar->enableSorting();
    }
    sidebar->blockSignals(false);

    setMediaCards(queryLibrary(sidebar->getState(), searchQuery));
}

}