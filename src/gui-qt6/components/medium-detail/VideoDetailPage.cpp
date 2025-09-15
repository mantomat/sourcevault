#include "VideoDetailPage.h"

#include "components/medium-detail/detail-sections/MediumDetailSection.h"
#include "components/medium-detail/detail-sections/UserDataDetailSection.h"
#include "components/medium-detail/detail-sections/VideoDetailSection.h"

#include <QVBoxLayout>

namespace Gui::Components {

VideoDetailPage::VideoDetailPage(const MediumDetailSection::Dependencies &mediumDeps,
                                 const UserDataDetailSection::Dependencies &userDataDeps,
                                 const VideoDetailSection::Dependencies &videoDeps, QWidget *parent)
    : DetailPage{parent}
    , thumbnail{new ThumbnailRenderer{std::nullopt, ":/assets/default-thumbnails/video.png", this}}
    , mediumSection{new MediumDetailSection{mediumDeps, this}}
    , userDataSection{new UserDataDetailSection{userDataDeps, this}}
    , videoSection{new VideoDetailSection{videoDeps, this}} {

    getContentLayout()->addWidget(thumbnail);
    getContentLayout()->addWidget(mediumSection);
    getContentLayout()->addWidget(videoSection);
    getContentLayout()->addWidget(userDataSection);
    getContentLayout()->addStretch();

    connect(this, &VideoDetailPage::saveRequested, this, &VideoDetailPage::refreshThumbnail);

    connect(mediumSection, &MediumDetailSection::stateChanged, this,
            &VideoDetailPage::onStateChanged);
    connect(videoSection, &VideoDetailSection::stateChanged, this,
            &VideoDetailPage::onStateChanged);
    connect(userDataSection, &UserDataDetailSection::stateChanged, this,
            &VideoDetailPage::onStateChanged);
};

auto VideoDetailPage::getMediumSection() const -> MediumDetailSection * {
    return mediumSection;
}

auto VideoDetailPage::getUserDataSection() const -> UserDataDetailSection * {
    return userDataSection;
}

auto VideoDetailPage::getVideoSection() const -> VideoDetailSection * {
    return videoSection;
}

void VideoDetailPage::setEditMode(bool isEditing) {
    DetailPage::setEditMode(isEditing);

    mediumSection->setEditMode(isEditing);
    userDataSection->setEditMode(isEditing);
    videoSection->setEditMode(isEditing);
}

void VideoDetailPage::refreshThumbnail() {
    thumbnail->trySetPreferred(videoSection->getState().videoUrl);
}

void VideoDetailPage::onStateChanged() {
    setSaveButtonDisabled(!(mediumSection->isEverythingValid() &&
                            videoSection->isEverythingValid() &&
                            userDataSection->isEverythingValid()));
}

}