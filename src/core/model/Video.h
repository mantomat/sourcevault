#ifndef VIDEO_H
#define VIDEO_H

#include "Medium.h"

#include <QDate>
#include <QUrl>

namespace Core::Model {

class Video : public Medium {

    ValidatedField<QUrl> videoUrl_{videoUrlValidator};
    ValidatedField<unsigned int> durationSeconds_{durationSecondsValidator};
    ValidatedField<QDate> uploadDate_{uploadDateValidator};
    ValidatedField<QUrl> thumbnailUrl_{thumbnailUrlValidator};

    /**
     * @brief Constructs a new Video.
     *
     * QUuid and QDate are passed by value since they are trivially copyable.
     */
    Video(QString &&title, QUuid id, QDate dateAdded);

  public:
    ~Video() override = default;
    Video(const Video &) = default;
    Video(Video &&) = default;
    auto operator=(const Video &) -> Video & = default;
    auto operator=(Video &&) -> Video & = default;

    auto operator==(const Video &) const -> bool = default;

    [[nodiscard]] auto clone() const -> std::unique_ptr<Medium> override;

    [[nodiscard]] static auto create(QString title, QUuid id = QUuid::createUuid(),
                                     QDate dateAdded = QDate::currentDate())
        -> std::optional<Video>;

    [[nodiscard]] auto videoUrl() -> ValidatedField<QUrl> &;
    [[nodiscard]] auto videoUrl() const -> const ValidatedField<QUrl> &;
    [[nodiscard]] static auto videoUrlValidator(const QUrl &urlToValidate) -> bool;

    [[nodiscard]] auto durationSeconds() -> ValidatedField<unsigned int> &;
    [[nodiscard]] auto durationSeconds() const -> const ValidatedField<unsigned int> &;
    [[nodiscard]] static auto durationSecondsValidator(unsigned int durationSecondsToValidate)
        -> bool;

    [[nodiscard]] auto uploadDate() -> ValidatedField<QDate> &;
    [[nodiscard]] auto uploadDate() const -> const ValidatedField<QDate> &;
    [[nodiscard]] static auto uploadDateValidator(const QDate &dateToValidate) -> bool;

    [[nodiscard]] auto thumbnailUrl() -> ValidatedField<QUrl> &;
    [[nodiscard]] auto thumbnailUrl() const -> const ValidatedField<QUrl> &;
    [[nodiscard]] static auto thumbnailUrlValidator(const QUrl &urlToValidate) -> bool;

    void accept(MediumVisitor &visitor) const override;
};

}

#endif
