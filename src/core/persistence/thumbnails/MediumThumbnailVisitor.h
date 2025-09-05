#ifndef MEDIUMTHUMBNAILVISITOR_H
#define MEDIUMTHUMBNAILVISITOR_H

#include "shared/MediumVisitor.h"

#include <QString>
#include <QUrl>

using Core::Shared::MediumVisitor;

namespace Core::Persistence::Thumbnails {

class MediumThumbnailVisitor : public MediumVisitor {

    std::optional<QUrl> localThumbnail;

  public:
    ~MediumThumbnailVisitor() override = default;
    MediumThumbnailVisitor() = default;
    MediumThumbnailVisitor(const MediumThumbnailVisitor &) = default;
    MediumThumbnailVisitor(MediumThumbnailVisitor &&) = default;
    auto operator=(const MediumThumbnailVisitor &) -> MediumThumbnailVisitor & = default;
    auto operator=(MediumThumbnailVisitor &&) -> MediumThumbnailVisitor & = default;

    /**
     * @return The path to the local thumbnail of the visited media if and only if
     * - the media has a `thumbnailUrl()` method,
     * - the resulting `QUrl` is local, valid: the file is present, readable and copyable.
     */
    [[nodiscard]] auto getLocalThumbnailPath() const -> std::optional<QString>;

    void visit(const Article & /*unused*/) override;
    void visit(const Book &book) override;
    void visit(const Video &video) override;
};

}

#endif