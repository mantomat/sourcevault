#include "ThumbnailsExporter.h"

#include "persistence/thumbnails/MediumThumbnailVisitor.h"
#include "shared/QUuidHasher.h"

#include <QDir>
#include <qfiledevice.h>
#include <unordered_set>

namespace Core::Persistence::Thumbnails {

ThumbnailsExporter::ThumbnailsExporter()
    : visitor{std::make_unique<MediumThumbnailVisitor>()} {}

auto ThumbnailsExporter::exportLocalThumbnails(const std::vector<const Medium *> &media,
                                               const QString &destinationDirPath) const
    -> std::variant<QFileDevice::FileError, std::unordered_set<QUuid>> {
    QDir destinationDir{destinationDirPath};

    if (!destinationDir.exists()) {
        return QFileDevice::PositionError;
    }

    const QString subDirNameStr{subDirName};
    if (!destinationDir.mkdir(subDirNameStr) || !destinationDir.cd(subDirNameStr)) {
        return QFileDevice::WriteError;
    }

    std::unordered_set<QUuid> copiedThumbnails{};
    for (const auto &medium : media) {
        medium->accept(*visitor);
        const auto thumbnailPath{visitor->getLocalThumbnailPath()};

        if (!thumbnailPath.has_value()) {
            continue;
        }

        QFile thumbnailFile{thumbnailPath.value()};
        const QFileInfo thumbnailFileInfo{thumbnailFile};

        const QString extension{thumbnailFileInfo.suffix()};

        const QString newFileName = medium->id().toString(QUuid::WithoutBraces) + "." + extension;
        const QString newFilePath = destinationDir.filePath(newFileName);

        if (!thumbnailFile.copy(newFilePath)) {
            return thumbnailFile.error();
        }
        copiedThumbnails.insert(medium->id());
    }
    return copiedThumbnails;
}

}