#ifndef VIDEODETAILSECTIONCONTROLLER_H
#define VIDEODETAILSECTIONCONTROLLER_H

#include "VideoDetailSection.h"
#include "model/Video.h"

#include <QObject>

using Core::Model::Video;

namespace Gui::Components {

class VideoSectionController : public QObject {
    Q_OBJECT

    VideoDetailSection *section;

  public:
    ~VideoSectionController() override = default;

    VideoSectionController(VideoDetailSection *newSection, const Video *videoToDisplay,
                           QObject *parent);

    VideoSectionController(const VideoSectionController &) = delete;
    VideoSectionController(VideoSectionController &&) = delete;
    auto operator=(const VideoSectionController &) -> VideoSectionController & = delete;
    auto operator=(VideoSectionController &&) -> VideoSectionController & = delete;

    void applyChanges(Video &videoToModify) const;

  private:
    void populateSection(const Video *videoToDisplay);
};

}

#endif