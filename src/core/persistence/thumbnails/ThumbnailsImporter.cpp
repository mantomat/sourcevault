#include "ThumbnailsImporter.h"

#include "persistence/thumbnails/MediumThumbnailSetter.h"

#include <QDir>
#include <QDirIterator>
#include <QUrl>

using Core::Model::Medium;

namespace Core::Persistence::Thumbnails {

ThumbnailsImporter::ThumbnailsImporter()
    : thumbnailSetter{std::make_unique<MediumThumbnailSetter>()} {}

auto ThumbnailsImporter::importLocalThumbnails(Library library,
                                               const QString &thumbnailsDirPath) const
    -> std::variant<QFileDevice::FileError, std::pair<Library, std::unordered_set<QUuid>>> {

    QDir dir{thumbnailsDirPath};
    if (!dir.exists()) {
        return QFileDevice::PositionError;
    }
    if (!dir.isReadable()) {
        return QFileDevice::ReadError;
    }

    const auto fileInfoList = dir.entryInfoList(QDir::Files);
    std::unordered_set<QUuid> setMediaIds;

    for (const QFileInfo &thumbnailInfo : fileInfoList) {
        const QUuid candidateId{thumbnailInfo.baseName()};

        if (candidateId.isNull()) {
            continue;
        }

        const std::optional<const Medium *> mediumOpt{library.getMedium(candidateId)};
        if (!mediumOpt.has_value()) {
            continue;
        }

        std::unique_ptr<Medium> newMedium{mediumOpt.value()->clone()};

        thumbnailSetter->setThumbnailUrl(QUrl::fromLocalFile(thumbnailInfo.filePath()));
        newMedium->accept(*thumbnailSetter);
        assert(thumbnailSetter->wasSuccessful());

        setMediaIds.insert(newMedium->id());
        const bool wasReplaced{library.replaceMedium(std::move(newMedium))};
        assert(wasReplaced);
    }

    return std::make_pair(std::move(library), std::move(setMediaIds));
}

}