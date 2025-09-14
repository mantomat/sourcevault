#ifndef EDITCONTROLLER_H
#define EDITCONTROLLER_H

#include "model/Medium.h"

#include <QObject>

using Core::Model::Medium;

namespace Gui::Components {

class DetailPageController : public QObject {
    Q_OBJECT

  public:
    using QObject::QObject;

    ~DetailPageController() override = default;

    DetailPageController(const DetailPageController &) = delete;
    DetailPageController(DetailPageController &&) = delete;
    auto operator=(const DetailPageController &) -> DetailPageController & = delete;
    auto operator=(DetailPageController &&) -> DetailPageController & = delete;

  signals:
    void mediumEdited(const Medium &updatedMedium);
    void goBackToLibraryRequest();
};

}

#endif