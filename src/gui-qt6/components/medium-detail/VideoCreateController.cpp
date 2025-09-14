#include "VideoCreateController.h"

#include "components/medium-detail/CreateController.h"

namespace Gui::Components {

VideoCreateController::VideoCreateController(VideoDetailPage *newView, QObject *parent)
    : CreateController{parent}
    , view{newView}
    , newVideo{Video::create("New Video").value()}
    , mediumSectionController{view->getMediumSection(), &newVideo, this}
    , videoSectionController{view->getVideoSection(), &newVideo, this}
    , userDataSectionController{view->getUserDataSection(), &newVideo, this} {
    view->setEditMode(true);

    connect(view, &DetailPage::saveRequested, this, &VideoCreateController::onSave);
    connect(view, &DetailPage::discardChanges, this, &VideoCreateController::creationCancelled);
}

void VideoCreateController::onSave() {
    mediumSectionController.applyChanges(newVideo);
    videoSectionController.applyChanges(newVideo);
    userDataSectionController.applyChanges(newVideo);

    emit mediumCreated(newVideo);
}

}