#include "DetailPageFactory.h"

#include "MediumToDetailPageVisitor.h"

namespace Gui {

auto DetailPageFactory::createDetailPage(const Core::Model::Medium *medium, QWidget *windowParent,
                                         QObject *controllerParent)
    -> std::pair<DetailPage *, DetailPageController *> {
    MediumToDetailPageVisitor visitor{windowParent, controllerParent};

    medium->accept(visitor);
    return visitor.getDetail();
}

}