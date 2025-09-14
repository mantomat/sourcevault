#ifndef VIDEODETAILSECTIONCONTROLLER_H
#define VIDEODETAILSECTIONCONTROLLER_H

#include "VideoDetailSection.h"
#include "model/Video.h"

#include <QObject>

using Core::Model::Video;

namespace Gui::Components {

class VideoDetailSectionController : public QObject {
    Q_OBJECT

    VideoDetailSection *section;

  public:
    ~VideoDetailSectionController() override = default;

    VideoDetailSectionController(VideoDetailSection *newSection, const Video *videoToDisplay,
                                 QObject *parent);

    VideoDetailSectionController(const VideoDetailSectionController &) = delete;
    VideoDetailSectionController(VideoDetailSectionController &&) = delete;
    auto operator=(const VideoDetailSectionController &) -> VideoDetailSectionController & = delete;
    auto operator=(VideoDetailSectionController &&) -> VideoDetailSectionController & = delete;

    void applyChanges(Video &videoToModify) const;

  private:
    void populateSection(const Video *videoToDisplay);
};

}

#endif