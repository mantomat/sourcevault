#ifndef DETAILPAGEFACTORY_H
#define DETAILPAGEFACTORY_H

#include "DialogsController.h"
#include "components/library-page/topbar/LibraryTopbar.h"
#include "components/medium-detail/CreateController.h"
#include "components/medium-detail/DetailPage.h"
#include "components/medium-detail/DetailPageController.h"
#include "components/medium-detail/detail-sections/ArticleDetailSection.h"
#include "components/medium-detail/detail-sections/BookDetailSection.h"
#include "components/medium-detail/detail-sections/MediumDetailSection.h"
#include "components/medium-detail/detail-sections/UserDataDetailSection.h"
#include "components/medium-detail/detail-sections/VideoDetailSection.h"

#include <QObject>

using Gui::Components::ArticleDetailSection;
using Gui::Components::BookDetailSection;
using Gui::Components::CreateController;
using Gui::Components::DetailPage;
using Gui::Components::DetailPageController;
using Gui::Components::LibraryTopbar;
using Gui::Components::MediumDetailSection;
using Gui::Components::UserDataDetailSection;
using Gui::Components::VideoDetailSection;

namespace Gui {

class DetailPageFactory : public QObject {
    Q_OBJECT

    static const MediumDetailSection::Dependencies mediumDeps;
    static const UserDataDetailSection::Dependencies userDataDeps;
    static const ArticleDetailSection::Dependencies articleDeps;
    static const BookDetailSection::Dependencies bookDeps;
    static const VideoDetailSection::Dependencies videoDeps;

  public:
    [[nodiscard]] static auto createDetailPage(const Core::Model::Medium *medium,
                                               DialogsController *dialogsController,
                                               QWidget *windowParent, QObject *controllerParent)
        -> std::pair<DetailPage *, DetailPageController *>;

    [[nodiscard]] static auto createNewMediumPage(LibraryTopbar::MediumTypeViewModel mediumType,
                                                  DialogsController *dialogsController,
                                                  QWidget *windowParent, QObject *controllerParent)
        -> std::pair<DetailPage *, CreateController *>;
};
}

#endif