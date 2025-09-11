#ifndef LIBRARYPAGECONTROLLER_H
#define LIBRARYPAGECONTROLLER_H

#include "LibraryPage.h"
#include "model/Library.h"
#include "model/Medium.h"
#include "queries/FilteringQuery.h"
#include "queries/Query.h"

#include <QObject>

using Core::Model::Library;
using Core::Model::Medium;
using Core::Queries::FilteringQuery;
using Core::Queries::Query;

namespace Gui::Components {

class LibraryPageController : public QObject {
    Q_OBJECT

    const Library *library;
    LibraryPage *libraryPage;

  public:
    LibraryPageController(LibraryPage *newLibraryPage, const Library *newLibrary, QObject *parent);

  signals:
    void mediumDetailsRequest(QUuid id);
    void mediumEditRequest(QUuid id);
    void mediumDeleteRequest(QUuid id);

  public slots:
    void refreshMediaList();

  private:
    void setMediaCards(const std::vector<const Medium *> &media) const;
    [[nodiscard]] auto queryLibrary(const LibrarySidebar::SidebarState &state,
                                    const QString &searchQuery) const
        -> std::vector<const Medium *>;

  private slots:
    void onQueryChanged();
};

}

#endif