#ifndef VIDEOCREATECONTROLLER_H
#define VIDEOCREATECONTROLLER_H

#include "components/medium-detail/CreateController.h"
#include "components/medium-detail/VideoDetailPage.h"
#include "components/medium-detail/detail-sections/MediumSectionController.h"
#include "components/medium-detail/detail-sections/UserDataSectionController.h"
#include "components/medium-detail/detail-sections/VideoDetailSectionController.h"

namespace Gui::Components {

class VideoCreateController : public CreateController {

    VideoDetailPage *view;

    Video newVideo;

    MediumSectionController mediumSectionController;
    VideoDetailSectionController videoSectionController;
    UserDataSectionController userDataSectionController;

  public:
    ~VideoCreateController() override = default;

    VideoCreateController(VideoDetailPage *newView, QObject *parent);

    VideoCreateController(const VideoCreateController &) = delete;
    VideoCreateController(VideoCreateController &&) = delete;
    auto operator=(const VideoCreateController &) -> VideoCreateController & = delete;
    auto operator=(VideoCreateController &&) -> VideoCreateController & = delete;

  private slots:
    void onSave();
};

}

#endif