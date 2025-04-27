#ifndef VIDEO_H
#define VIDEO_H

#include "Medium.h"
#include "OptionalField.h"

#include <QDate>
#include <QUrl>

namespace Core::Model {

class Video : public Medium {

  public:
    ~Video() override = default;

    ValidatedField<QUrl>& videoUrl();
    const ValidatedField<QUrl>& videoUrl() const;
    static bool videoUrlValidator(const QUrl& url);

    OptionalField<unsigned int>& durationSeconds();
    const OptionalField<unsigned int>& durationSeconds() const;

    ValidatedField<QDate>& uploadDate();
    const ValidatedField<QDate>& uploadDate() const;
    static bool uploadDateValidator(const QDate& date);

    ValidatedField<QUrl>& thumbnailUrl();
    const ValidatedField<QUrl>& thumbnailUrl() const;
    static bool thumbnailUrlValidator(const QUrl& url);

  private:
    ValidatedField<QUrl> videoUrl_{videoUrlValidator};
    OptionalField<unsigned int> durationSeconds_;
    ValidatedField<QDate> uploadDate_{uploadDateValidator};
    ValidatedField<QUrl> thumbnailUrl_{thumbnailUrlValidator};
};

}

#endif
