#include "LibraryPageController.h"

#include "components/library-page/MediumToCardVisitor.h"
#include "model/Book.h"
#include "queries/FilteringQuery.h"
#include "queries/SearchQuery.h"
#include "queries/SortingQuery.h"
#include "queries/filters/FavoriteFilter.h"
#include "queries/filters/MediumTypeFilter.h"
#include "queries/search/SearchEngine.h"
#include "queries/sortings/DateAddedSort.h"
#include "queries/sortings/Sort.h"
#include "queries/sortings/TitleSort.h"

#include <ranges>

using Core::Queries::Query;
using Core::Queries::SearchQuery;
using Core::Queries::SortingQuery;
using Core::Queries::Filters::FavoriteFilter;
using Core::Queries::Filters::MediumTypeFilter;
using Core::Queries::Sortings::DateAddedSort;
using Core::Queries::Sortings::TitleSort;

using Core::Model::Book;

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

        auto article{Article::make("An article").value()};
        article->authors().set({"Author 3", "Author 4"});
        article->userData().topics().set({"topic 2"});

        auto video{Video::make("My video").value()};
        video->authors().set({"Author 1", "Author 4"});

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
}

void LibraryPageController::onLibraryMergeRequest(const Library &libraryToMerge) {
    library.merge(libraryToMerge);
}

void LibraryPageController::onMediumEdited(const Medium &medium) {
    library.replaceMedium(medium.clone());
    refreshSidebarTopicsList();
}

auto LibraryPageController::toMediumCards(std::vector<const Medium *> media)
    -> std::vector<LibraryMediumCard::MediumCardViewModel> {

    MediumToCardVisitor visitor;
    auto mediumCardsView{media | std::views::transform([&visitor](const auto &medium) {
                             medium->accept(visitor);
                             return visitor.getCardData();
                         })};
    return std::vector<LibraryMediumCard::MediumCardViewModel>{mediumCardsView.begin(),
                                                               mediumCardsView.end()};
}

void LibraryPageController::setMediaCards(const std::vector<const Medium *> &media) const {
    libraryPage->getMediaList()->setMedia(toMediumCards(media));
}

void LibraryPageController::refreshSidebarTopicsList() const {
    libraryPage->getSidebar()->setAvailableTopics(library.getTopicsUnion());
}

auto LibraryPageController::queryLibrary(const LibrarySidebar::SidebarState &state,
                                         const QString &searchQuery) const
    -> std::vector<const Medium *> {
    std::unique_ptr query{sidebarAndSearchStateToQuery(state, searchQuery)};

    return query->query(library.getMedia());
}

auto LibraryPageController::sidebarAndSearchStateToQuery(const LibrarySidebar::SidebarState &state,
                                                         const QString &searchQuery)
    -> std::unique_ptr<Query> {
    std::unique_ptr<Query> filter;
    if (!searchQuery.isEmpty()) {
        assert(state.sort == LibrarySidebar::SortTypeOptions::Disabled);
        return sidebarAndSearchStateToSearchQuery(state, searchQuery);
    }

    assert(state.sort != LibrarySidebar::SortTypeOptions::Disabled);
    return sidebarStateToSortingQuery(state);
}

auto LibraryPageController::sidebarAndSearchStateToSearchQuery(
    const LibrarySidebar::SidebarState &state, const QString &searchQuery)
    -> std::unique_ptr<Query> {
    FilteringQuery filtering{sidebarStateToFilteringQuery(state)};

    return std::make_unique<SearchQuery>(std::move(filtering), SearchEngine{searchQuery});
}

auto LibraryPageController::sidebarStateToSortingQuery(const LibrarySidebar::SidebarState &state)
    -> std::unique_ptr<Query> {
    FilteringQuery filtering{sidebarStateToFilteringQuery(state)};
    std::unique_ptr<Sort> sort;

    switch (state.sort) {
    case LibrarySidebar::SortTypeOptions::TitleAsc:
        sort = std::make_unique<TitleSort>(true);
        break;
    case LibrarySidebar::SortTypeOptions::TitleDesc:
        sort = std::make_unique<TitleSort>(false);
        break;
    case LibrarySidebar::SortTypeOptions::DateAddedDesc:
        sort = std::make_unique<DateAddedSort>(true);
        break;
    case LibrarySidebar::SortTypeOptions::DateAddedAsc:
        sort = std::make_unique<DateAddedSort>(false);
        break;
    case LibrarySidebar::SortTypeOptions::Disabled:
        qWarning() << "This place should never be reached";
        assert(false);
        break;
    }

    return std::make_unique<SortingQuery>(
        SortingQuery::create(std::move(filtering), std::move(sort)).value());
}

auto LibraryPageController::sidebarStateToFilteringQuery(const LibrarySidebar::SidebarState &state)
    -> FilteringQuery {
    std::vector<std::unique_ptr<const Filter>> filters;

    switch (state.mediumTypeFilter) {
    case LibrarySidebar::MediumTypeFilterOptions::Article:
        filters.push_back(std::make_unique<MediumTypeFilter>(
            std::unordered_set<std::type_index>{typeid(Article)}));
        break;
    case LibrarySidebar::MediumTypeFilterOptions::Book:
        filters.push_back(
            std::make_unique<MediumTypeFilter>(std::unordered_set<std::type_index>{typeid(Book)}));
        break;
    case LibrarySidebar::MediumTypeFilterOptions::Video:
        filters.push_back(
            std::make_unique<MediumTypeFilter>(std::unordered_set<std::type_index>{typeid(Video)}));
        break;
    case LibrarySidebar::MediumTypeFilterOptions::NoFilter:
        break;
    }

    if (state.onlyFavoriteFilter) {
        filters.push_back(std::make_unique<FavoriteFilter>());
    }

    // TODO model minimumPriorityFilter, topicsFilter

    return FilteringQuery::create(std::move(filters)).value();
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

    setMediaCards(queryLibrary(sidebar->getState(), searchQuery));

    sidebar->blockSignals(false);
}

void LibraryPageController::onMediumDeletionRequested(const QUuid &id) {
    libraryPage->getMediaList()->removeFromList(id);

    const bool result{library.removeMedium(id)};
    assert(result);
    refreshSidebarTopicsList();
}

}