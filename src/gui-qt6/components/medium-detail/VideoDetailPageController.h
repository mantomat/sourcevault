#ifndef VIDEOEDITCONTROLLER_H
#define VIDEOEDITCONTROLLER_H

#include "components/medium-detail/DetailPageController.h"
#include "components/medium-detail/VideoDetailPage.h"
#include "components/medium-detail/detail-sections/MediumSectionController.h"
#include "components/medium-detail/detail-sections/UserDataSectionController.h"
#include "components/medium-detail/detail-sections/VideoDetailSectionController.h"
#include "model/Video.h"

using Core::Model::Video;

namespace Gui::Components {

class VideoDetailPageController : public DetailPageController {
    Q_OBJECT

    VideoDetailPage *view;

    const Video *originalVideo;
    Video editableVideo;

    MediumSectionController mediumSectionController;
    VideoDetailSectionController videoSectionController;
    UserDataSectionController userDataSectionController;

  public:
    ~VideoDetailPageController() override = default;

    VideoDetailPageController(VideoDetailPage *newView, const Video *newOriginalVideo,
                              QObject *parent);

    VideoDetailPageController(const VideoDetailPageController &) = delete;
    VideoDetailPageController(VideoDetailPageController &&) = delete;
    auto operator=(const VideoDetailPageController &) -> VideoDetailPageController & = delete;
    auto operator=(VideoDetailPageController &&) -> VideoDetailPageController & = delete;

  private slots:
    void onSave();
    void onEditRequested();
    void onDiscardChanges();
};

}

#endif