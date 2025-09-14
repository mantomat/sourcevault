#include "VideoDetailPageController.h"

namespace Gui::Components {

VideoDetailPageController::VideoDetailPageController(VideoDetailPage *newView,
                                                     const Video *newOriginalVideo, QObject *parent)
    : DetailPageController(parent)
    , view{newView}
    , originalVideo{newOriginalVideo}
    , editableVideo{*originalVideo}
    , mediumSectionController{view->getMediumSection(), originalVideo, this}
    , videoSectionController{view->getVideoSection(), originalVideo, this}
    , userDataSectionController{view->getUserDataSection(), originalVideo, this} {

    view->setEditMode(false);

    connect(view, &DetailPage::saveRequested, this, &VideoDetailPageController::onSave);
    connect(view, &DetailPage::editRequested, this, &VideoDetailPageController::onEditRequested);
    connect(view, &DetailPage::discardChanges, this, &VideoDetailPageController::onDiscardChanges);
    connect(view, &DetailPage::goBackToLibrary, this,
            &VideoDetailPageController::goBackToLibraryRequest);
}

void VideoDetailPageController::onSave() {
    mediumSectionController.applyChanges(editableVideo);
    videoSectionController.applyChanges(editableVideo);
    userDataSectionController.applyChanges(editableVideo);

    emit mediumEdited(editableVideo);
}

void VideoDetailPageController::onEditRequested() {
    view->setEditMode(true);
}

void VideoDetailPageController::onDiscardChanges() {
    view->setEditMode(false);
}

}