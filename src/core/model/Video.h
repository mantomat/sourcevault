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

    Video(QString&& title, QUuid id, QDate dateAdded);

  public:
    Video(const Video&) = default;
    Video(Video&&) = default;
    ~Video() override = default;

    static std::optional<Video> create(QString title, QUuid id = QUuid::createUuid(),
                                       QDate dateAdded = QDate::currentDate());

    ValidatedField<QUrl>& videoUrl();
    const ValidatedField<QUrl>& videoUrl() const;
    static bool videoUrlValidator(const QUrl& urlToValidate);

    ValidatedField<unsigned int>& durationSeconds();
    const ValidatedField<unsigned int>& durationSeconds() const;
    static bool durationSecondsValidator(unsigned int durationSecondsToValidate);

    ValidatedField<QDate>& uploadDate();
    const ValidatedField<QDate>& uploadDate() const;
    static bool uploadDateValidator(const QDate& dateToValidate);

    ValidatedField<QUrl>& thumbnailUrl();
    const ValidatedField<QUrl>& thumbnailUrl() const;
    static bool thumbnailUrlValidator(const QUrl& urlToValidate);
};

}

#endif
