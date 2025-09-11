#include "LibraryPageController.h"

#include "LibraryQueryBuilder.h"
#include "MediumCardsViewModelFactory.h"
#include "model/Book.h"
#include "queries/FilteringQuery.h"
#include "queries/filters/MediumTypeFilter.h"
#include "queries/search/SearchEngine.h"
#include "queries/sortings/Sort.h"

using Core::Model::MediumUserData;

namespace Gui::Components {

LibraryPageController::LibraryPageController(LibraryPage *newLibraryPage, QObject *parent)
    : QObject{parent}
    , libraryPage{newLibraryPage} {

    ///////////////////////////////////////
    // TODO remove, just for testing...
    for (int i = 0; i < 20; i++) {
        auto book{Book::make("A Book").value()};
        book->authors().set({"Author 1", "Author 2"});
        book->userData().topics().set({"topic 1"});
        book->userData().priority().set(MediumUserData::PriorityLevel::mid);

        auto article{Article::make("An article").value()};
        article->authors().set({"Author 3", "Author 4"});
        article->userData().topics().set({"topic 2"});
        article->userData().priority().set(MediumUserData::PriorityLevel::low);

        auto video{Video::make("My video").value()};
        video->authors().set({"Author 1", "Author 4"});
        video->userData().priority().set(MediumUserData::PriorityLevel::high);

        library.addMedium(std::move(book));
        library.addMedium(std::move(article));
        library.addMedium(std::move(video));
    }
    auto book{Book::make("Favorite With notes").value()};
    book->userData().notes().set("ciao come va?");
    book->userData().favorite() = true;
    library.addMedium(std::move(book));

    setMediaCards(library.getMedia());
    refreshSidebarTopicsList();
    ///////////////////////////////////////

    connect(libraryPage->getTopbar(), &LibraryTopbar::searchQueryChanged, this,
            &LibraryPageController::onQueryChanged);
    connect(libraryPage->getSidebar(), &LibrarySidebar::stateChanged, this,
            &LibraryPageController::onQueryChanged);
}

void LibraryPageController::onLibraryImportRequest(const Library &newLibrary) {
    library = newLibrary;
    setMediaCards(library.getMedia());
    refreshSidebarTopicsList();
}

void LibraryPageController::onLibraryMergeRequest(const Library &libraryToMerge) {
    library.merge(libraryToMerge);
    setMediaCards(library.getMedia());
    refreshSidebarTopicsList();
}

void LibraryPageController::onMediumEdited(const Medium &medium) {
    library.replaceMedium(medium.clone());
    setMediaCards(library.getMedia());
    refreshSidebarTopicsList();
}

void LibraryPageController::setMediaCards(const std::vector<const Medium *> &media) const {
    libraryPage->getMediaList()->setMedia(MediumCardsViewModelFactory::createMediumCards(media));
}

void LibraryPageController::refreshSidebarTopicsList() const {
    libraryPage->getSidebar()->setAvailableTopics(library.getTopicsUnion());
}

auto LibraryPageController::queryLibrary(const LibrarySidebar::SidebarState &state,
                                         const QString &searchQuery) const
    -> std::vector<const Medium *> {
    std::unique_ptr query{LibraryQueryBuilder::build(state, searchQuery)};
    return query->query(library.getMedia());
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

void LibraryPageController::onMediumDeletionRequested(const QUuid &id) {
    libraryPage->getMediaList()->removeFromList(id);

    const bool result{library.removeMedium(id)};
    assert(result);
    refreshSidebarTopicsList();
}

}