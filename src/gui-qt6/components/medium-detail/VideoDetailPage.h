#ifndef VIDEODETAILPAGE_H
#define VIDEODETAILPAGE_H

#include "components/medium-detail/DetailPage.h"
#include "components/medium-detail/detail-sections/MediumDetailSection.h"
#include "components/medium-detail/detail-sections/UserDataDetailSection.h"
#include "components/medium-detail/detail-sections/VideoDetailSection.h"
#include "components/thumbnail-renderer/ThumbnailRenderer.h"

#include <QWidget>

namespace Gui::Components {

class VideoDetailPage : public DetailPage {

    ThumbnailRenderer *thumbnail;

    MediumDetailSection *mediumSection;
    UserDataDetailSection *userDataSection;
    VideoDetailSection *videoSection;

  public:
    ~VideoDetailPage() override = default;

    VideoDetailPage(const MediumDetailSection::Dependencies &mediumDeps,
                    const UserDataDetailSection::Dependencies &userDataDeps,
                    const VideoDetailSection::Dependencies &videoDeps, QWidget *parent);

    [[nodiscard]] auto getMediumSection() const -> MediumDetailSection *;
    [[nodiscard]] auto getUserDataSection() const -> UserDataDetailSection *;
    [[nodiscard]] auto getVideoSection() const -> VideoDetailSection *;

    VideoDetailPage(const VideoDetailPage &) = delete;
    VideoDetailPage(VideoDetailPage &&) = delete;
    auto operator=(const VideoDetailPage &) -> VideoDetailPage & = delete;
    auto operator=(VideoDetailPage &&) -> VideoDetailPage & = delete;

  public slots:
    void setEditMode(bool isEditing) override;

  private slots:
    void refreshThumbnail();
    void onStateChanged();
};

}

#endif