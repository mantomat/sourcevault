#include "JsonPersistenceManager.h"

#include "persistence/json/deserialization/JsonDeserializationError.h"

#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <variant>

namespace Core::Persistence::Json {

JsonPersistenceManager::JsonPersistenceManager(MediaSerializationConfigs newConfigs)
    : deserializer{newConfigs}
    , serializer{std::move(newConfigs)} {}

auto JsonPersistenceManager::importLibrary(const QString &filePath) const
    -> std::variant<QFileDevice::FileError, JsonDeserializationError, Library> {

    QFile file{filePath};
    if (!file.open(QIODevice::ReadOnly)) {
        return file.error();
    }

    const QByteArray jsonData{file.readAll()};
    const QJsonDocument jsonDocument{QJsonDocument::fromJson(jsonData)};

    auto deserializationResult = deserializer.deserialize(jsonDocument);

    return std::visit(
        [](auto &&arg) -> std::variant<QFileDevice::FileError, JsonDeserializationError, Library> {
            return std::forward<decltype(arg)>(arg);
        },
        std::move(deserializationResult));
}

auto JsonPersistenceManager::exportMedia(const std::vector<const Medium *> &media,
                                         const QString &filePath) const
    -> std::optional<QFileDevice::FileError> {
    QJsonDocument jsonDocument{serializer.serialize(media)};
    const QByteArray jsonData{jsonDocument.toJson()};

    QFile file{filePath};
    if (!file.open(QIODevice::WriteOnly)) {
        return file.error();
    }

    if (file.write(jsonDocument.toJson()) == -1) {
        return file.error();
    }

    return std::nullopt;
}

}