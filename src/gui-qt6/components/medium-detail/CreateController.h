#ifndef CREATECONTROLLER_H
#define CREATECONTROLLER_H

#include "model/Medium.h"

#include <QObject>

using Core::Model::Medium;

namespace Gui::Components {

class CreateController : public QObject {
    Q_OBJECT

  public:
    using QObject::QObject;

    virtual ~CreateController() = default;

    CreateController(const CreateController &) = delete;
    CreateController(CreateController &&) = delete;
    auto operator=(const CreateController &) -> CreateController & = delete;
    auto operator=(CreateController &&) -> CreateController & = delete;

  signals:
    void mediumCreated(const Medium &newMedium);
    void creationCancelled();
};
}

#endif