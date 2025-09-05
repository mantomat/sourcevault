#ifndef JSONPERSISTENCEMANAGER_H
#define JSONPERSISTENCEMANAGER_H

#include "persistence/MediaSerializationConfigs.h"
#include "persistence/json/deserialization/JsonDeserializationError.h"
#include "persistence/json/deserialization/JsonMediaDeserializer.h"
#include "persistence/json/serialization/JsonMediaSerializer.h"

#include <QFileDevice>

namespace Core::Persistence::Json {

class JsonPersistenceManager {

    JsonMediaDeserializer deserializer;
    JsonMediaSerializer serializer;

  public:
    JsonPersistenceManager(MediaSerializationConfigs newConfigs = MediaSerializationConfigs{});

    /**
     * @brief Imports a `Library` from a json file.
     * @return The `Library` in case of success. A `FileError` if a file system error occurs. A
     * `JsonDeserializationError` if the deserialization process fails.
     */
    [[nodiscard]] auto importLibrary(const QString &filePath) const
        -> std::variant<QFileDevice::FileError, JsonDeserializationError, Library>;

    /**
     * @brief Exports a vector of media from a json file.
     *
     * @param media A view on some `Library`'s media, with all the related invariants.
     *
     * @return `std::nullopt` if successful. `FileError` otherwise.
     */
    [[nodiscard]] auto exportMedia(const std::vector<const Medium *> &media,
                                   const QString &filePath) const
        -> std::optional<QFileDevice::FileError>;
};

}

#endif