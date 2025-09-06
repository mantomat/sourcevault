#ifndef THUMBNAILSIMPORTER_H
#define THUMBNAILSIMPORTER_H

#include "model/Library.h"
#include "persistence/thumbnails/MediumThumbnailSetter.h"

#include <QFileDevice>
#include <QFileInfo>
#include <QString>
#include <unordered_set>

using Core::Model::Library;

namespace Core::Persistence::Thumbnails {

class ThumbnailsImporter {

    std::unique_ptr<MediumThumbnailSetter> thumbnailSetter;

  public:
    ThumbnailsImporter();

    /**
     * @brief Iterates through the directory at `thumbnailsDirPath` and sets all the corresponding
     * media `thumbnailUrl`s to point to the ones found in it.
     *
     * A thumbnail is set (and, in case, overridden) if and only if the image is called
     * `mediumId.XXX` and `mediumId` matches an `id` in the `library`.
     *
     * @return The new library and medium ids of the thumbnails that have been actually set, or a
     * `FileError` otherwise.
     */
    [[nodiscard]] auto importLocalThumbnails(Library library,
                                             const QString &thumbnailsDirPath) const
        -> std::variant<QFileDevice::FileError, std::pair<Library, std::unordered_set<QUuid>>>;
};

}

#endif