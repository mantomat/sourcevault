#ifndef LIBRARYQUERYBUILDER_H
#define LIBRARYQUERYBUILDER_H

#include "model/Medium.h"
#include "queries/FilteringQuery.h"
#include "queries/Query.h"
#include "sidebar/LibrarySidebar.h"

using Core::Model::Medium;
using Core::Queries::FilteringQuery;
using Core::Queries::Query;

namespace Gui::Components {

class LibraryQueryBuilder {
  public:
    [[nodiscard]] static auto build(const LibrarySidebar::SidebarState &state,
                                    const QString &searchQuery) -> std::unique_ptr<Query>;

  private:
    [[nodiscard]] static auto
    sidebarAndSearchStateToSearchQuery(const LibrarySidebar::SidebarState &state,
                                       const QString &searchQuery) -> std::unique_ptr<Query>;

    [[nodiscard]] static auto sidebarStateToSortingQuery(const LibrarySidebar::SidebarState &state)
        -> std::unique_ptr<Query>;

    [[nodiscard]] static auto
    sidebarStateToFilteringQuery(const LibrarySidebar::SidebarState &state) -> FilteringQuery;
};

}

#endif