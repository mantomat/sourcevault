#include "MediumToDetailPageVisitor.h"

#include "components/medium-detail/VideoDetailPage.h"
#include "components/medium-detail/VideoDetailPageController.h"
#include "components/medium-detail/detail-sections/VideoDetailSection.h"
#include "model/Medium.h"
#include "model/MediumUserData.h"
#include "model/Video.h"

using Core::Model::Medium;
using Core::Model::MediumUserData;
using Core::Model::Video;
using Gui::Components::VideoDetailPage;
using Gui::Components::VideoDetailPageController;
using Gui::Components::VideoDetailSection;

namespace Gui {

MediumToDetailPageVisitor::MediumToDetailPageVisitor(QWidget *newWindowParent,
                                                     QObject *newControllerParent)
    : windowParent{newWindowParent}
    , controllerParent{newControllerParent}
    , mediumDeps{MediumDetailSection::Dependencies{
          .titleValidator = &Medium::titleValidator,
          .authorValidator = &Medium::authorValidator,
          .languageValidator = &Medium::languageValidator,
      }}
    , userDataDeps{UserDataDetailSection::Dependencies{
          .notesValidator = &MediumUserData::notesValidator,
          .topicValidator = &MediumUserData::topicValidator,
      }} {}

auto MediumToDetailPageVisitor::getDetail() const
    -> std::pair<DetailPage *, DetailPageController *> {
    return std::make_pair(detailPage, detailPageController);
}

void MediumToDetailPageVisitor::visit(const Article &article) {
    // TODO
    Q_UNUSED(article);
}
void MediumToDetailPageVisitor::visit(const Book &book) {
    // TODO
    Q_UNUSED(book);
}
void MediumToDetailPageVisitor::visit(const Video &video) {
    detailPage =
        new VideoDetailPage{mediumDeps, userDataDeps,
                            VideoDetailSection::Dependencies{
                                .videoUrlValidator = &Video::videoUrlValidator,
                                .durationSecondsValidator = &Video::durationSecondsValidator,
                                .uploadDateValidator = &Video::uploadDateValidator,
                                .thumbnailUrlValidator = &Video::thumbnailUrlValidator},
                            windowParent};

    detailPageController = new VideoDetailPageController{
        dynamic_cast<VideoDetailPage *>(detailPage), &video, controllerParent};
}

}