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

    Library library;
    LibraryPage *libraryPage;

  public:
    LibraryPageController(LibraryPage *newLibraryPage, QObject *parent);

  signals:
    void mediumDetailsRequest(const Medium *medium);
    void mediumEditRequest(const Medium *medium);

  public slots:
    void onLibraryImportRequest(const Library &newLibrary);
    void onLibraryMergeRequest(const Library &libraryToMerge);
    void onMediumEdited(const Medium &medium);

  private:
    void setMediaCards(const std::vector<const Medium *> &media) const;
    void refreshSidebarTopicsList() const;

    auto queryLibrary(const LibrarySidebar::SidebarState &state, const QString &searchQuery) const
        -> std::vector<const Medium *>;

  private slots:
    void onQueryChanged();
    void onMediumDeletionRequested(const QUuid &id);
};

}

#endif