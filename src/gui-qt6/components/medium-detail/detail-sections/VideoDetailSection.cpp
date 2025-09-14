#include "VideoDetailSection.h"

#include "components/medium-detail/field-editors/UrlFieldEditor.h"

#include <QFormLayout>
#include <QVBoxLayout>

namespace Gui::Components {

VideoDetailSection::VideoDetailSection(const Dependencies &deps, QWidget *parent)
    : QWidget{parent}
    , videoUrlEditor{new UrlFieldEditor{deps.videoUrlValidator, false, this}}
    , durationSecondsEditor{new NumberFieldEditor{deps.durationSecondsValidator, this}}
    , uploadDateEditor{new DateFieldEditor{deps.uploadDateValidator, this}}
    , thumbnailUrlEditor{new UrlFieldEditor{deps.thumbnailUrlValidator, true, this}} {

    auto *layout{new QFormLayout{this}};
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addRow("Video URL:", videoUrlEditor);
    layout->addRow("Duration (seconds):", durationSecondsEditor);
    layout->addRow("Upload date:", uploadDateEditor);
    layout->addRow("Thumbnail:", thumbnailUrlEditor);

    connect(videoUrlEditor, &UrlFieldEditor::stateChanged, this, &VideoDetailSection::stateChanged);
    connect(durationSecondsEditor, &NumberFieldEditor::stateChanged, this,
            &VideoDetailSection::stateChanged);
    connect(uploadDateEditor, &DateFieldEditor::stateChanged, this,
            &VideoDetailSection::stateChanged);
    connect(videoUrlEditor, &UrlFieldEditor::stateChanged, this, &VideoDetailSection::stateChanged);
}

auto VideoDetailSection::isEverythingValid() const -> bool {
    return videoUrlEditor->isValid() && durationSecondsEditor->isValid() &&
           uploadDateEditor->isValid() && thumbnailUrlEditor->isValid();
}

auto VideoDetailSection::getState() const -> VideoDetailViewModel {
    return VideoDetailViewModel{.videoUrl = videoUrlEditor->url(),
                                .durationSeconds = durationSecondsEditor->number(),
                                .uploadDate = uploadDateEditor->date(),
                                .thumbnailUrl = thumbnailUrlEditor->url()};
}

void VideoDetailSection::setState(const VideoDetailViewModel &initData) {
    videoUrlEditor->setUrl(initData.videoUrl);
    durationSecondsEditor->setNumber(initData.durationSeconds);
    uploadDateEditor->setDate(initData.uploadDate);
    thumbnailUrlEditor->setUrl(initData.thumbnailUrl);
}

void VideoDetailSection::setEditMode(bool isEditing) {
    videoUrlEditor->setEditMode(isEditing);
    durationSecondsEditor->setEditMode(isEditing);
    uploadDateEditor->setEditMode(isEditing);
    thumbnailUrlEditor->setEditMode(isEditing);
}

}