#ifndef MEDIUMTHUMBNAILSETTER_H
#define MEDIUMTHUMBNAILSETTER_H

#include "shared/MutMediumVisitor.h"

#include <QUrl>

using Core::Shared::MutMediumVisitor;

namespace Core::Persistence::Thumbnails {

class MediumThumbnailSetter : public MutMediumVisitor {

    QUrl thumbnailUrlToSet;
    bool wasSet{false};

  public:
    ~MediumThumbnailSetter() override = default;
    MediumThumbnailSetter() = default;
    MediumThumbnailSetter(const MediumThumbnailSetter &) = default;
    MediumThumbnailSetter(MediumThumbnailSetter &&) = default;
    auto operator=(const MediumThumbnailSetter &) -> MediumThumbnailSetter & = default;
    auto operator=(MediumThumbnailSetter &&) -> MediumThumbnailSetter & = default;

    void setThumbnailUrl(QUrl newThumbnailUrl);

    [[nodiscard]] auto wasSuccessful() const -> bool;

    void visit(Article & /*unused*/) override;
    void visit(Book &book) override;
    void visit(Video &video) override;
};

}

#endif