#include "VideoSectionController.h"

namespace Gui::Components {

VideoSectionController::VideoSectionController(VideoDetailSection *newSection,
                                               const Video *videoToDisplay, QObject *parent)
    : QObject{parent}
    , section{newSection} {
    populateSection(videoToDisplay);
}

void VideoSectionController::applyChanges(Video &videoToModify) const {
    assert(section->isEverythingValid());

    const auto formData{section->getState()};

    if (formData.videoUrl.has_value()) {
        videoToModify.videoUrl().set(formData.videoUrl.value());
    } else {
        videoToModify.videoUrl().unset();
    }

    if (formData.durationSeconds.has_value()) {
        videoToModify.durationSeconds().set(formData.durationSeconds.value());
    } else {
        videoToModify.durationSeconds().unset();
    }

    if (formData.uploadDate.has_value()) {
        videoToModify.uploadDate().set(formData.uploadDate.value());
    } else {
        videoToModify.uploadDate().unset();
    }

    if (formData.thumbnailUrl.has_value()) {
        videoToModify.thumbnailUrl().set(formData.thumbnailUrl.value());
    } else {
        videoToModify.thumbnailUrl().unset();
    }
}

void VideoSectionController::populateSection(const Video *videoToDisplay) {
    section->setState({.videoUrl = videoToDisplay->videoUrl().get(),
                       .durationSeconds = videoToDisplay->durationSeconds().get(),
                       .uploadDate = videoToDisplay->uploadDate().get(),
                       .thumbnailUrl = videoToDisplay->thumbnailUrl().get()});
}

}