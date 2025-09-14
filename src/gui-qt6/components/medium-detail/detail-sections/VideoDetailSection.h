#ifndef VIDEODETAILSECTION_H
#define VIDEODETAILSECTION_H

#include "components/medium-detail/field-editors/DateFieldEditor.h"
#include "components/medium-detail/field-editors/NumberFieldEditor.h"
#include "components/medium-detail/field-editors/UrlFieldEditor.h"

#include <QDate>
#include <QWidget>

namespace Gui::Components {

class VideoDetailSection : public QWidget {
    Q_OBJECT

  public:
    struct VideoDetailViewModel {
        std::optional<QUrl> videoUrl;
        std::optional<unsigned int> durationSeconds;
        std::optional<QDate> uploadDate;
        std::optional<QUrl> thumbnailUrl;
    };

    struct Dependencies {
        std::function<bool(const QUrl &)> videoUrlValidator;
        std::function<bool(const unsigned int &)> durationSecondsValidator;
        std::function<bool(const QDate &)> uploadDateValidator;
        std::function<bool(const QUrl &)> thumbnailUrlValidator;
    };

  private:
    UrlFieldEditor *videoUrlEditor;
    NumberFieldEditor *durationSecondsEditor;
    DateFieldEditor *uploadDateEditor;
    UrlFieldEditor *thumbnailUrlEditor;

  public:
    ~VideoDetailSection() override = default;

    VideoDetailSection(const Dependencies &deps, QWidget *parent);

    VideoDetailSection(const VideoDetailSection &) = delete;
    VideoDetailSection(VideoDetailSection &&) = delete;
    auto operator=(const VideoDetailSection &) -> VideoDetailSection & = delete;
    auto operator=(VideoDetailSection &&) -> VideoDetailSection & = delete;

    [[nodiscard]] auto isEverythingValid() const -> bool;

  public slots:
    [[nodiscard]] auto getState() const -> VideoDetailViewModel;
    void setState(const VideoDetailViewModel &initData);
    void setEditMode(bool isEditing);

  signals:
    void stateChanged();
};

}

#endif