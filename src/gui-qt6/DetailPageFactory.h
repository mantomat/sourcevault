#ifndef DETAILPAGEFACTORY_H
#define DETAILPAGEFACTORY_H

#include "components/medium-detail/DetailPage.h"
#include "components/medium-detail/DetailPageController.h"

#include <QObject>

using Gui::Components::DetailPage;
using Gui::Components::DetailPageController;

namespace Gui {

class DetailPageFactory : public QObject {
    Q_OBJECT

  public:
    [[nodiscard]] static auto createDetailPage(const Core::Model::Medium *medium,

                                               QWidget *windowParent, QObject *controllerParent)
        -> std::pair<DetailPage *, DetailPageController *>;

    // TODO
    // [[nodiscard]] static auto createNewMediumPage(MediumType enum , QWidget *parent);
};

}

#endif