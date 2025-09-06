#ifndef THUMBNAILSEXPORTER_H
#define THUMBNAILSEXPORTER_H

#include "model/Medium.h"
#include "persistence/thumbnails/MediumThumbnailVisitor.h"

#include <QFileDevice>
#include <QUuid>
#include <unordered_set>

using Core::Model::Medium;

namespace Core::Persistence::Thumbnails {

class ThumbnailsExporter {

    std::unique_ptr<MediumThumbnailVisitor> visitor;

  public:
    ThumbnailsExporter();

    static constexpr const char *subDirName{"sourcevault-thumbnails"};

    /**
     * @brief Copies all the available local thumbnails to a subfolder of the given
     * `destinationFolder`, naming them `mediumId.***`.
     *
     * @return The medium ids of the thumbnails that have been actually copied, or a `FileError`
     * otherwise.
     */
    [[nodiscard]] auto exportLocalThumbnails(const std::vector<const Medium *> &media,
                                             const QString &destinationDirPath) const
        -> std::variant<QFileDevice::FileError, std::unordered_set<QUuid>>;
};

}

#endif