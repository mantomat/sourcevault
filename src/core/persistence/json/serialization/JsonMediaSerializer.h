#ifndef JSONMEDIASERIALIZER_H
#define JSONMEDIASERIALIZER_H

#include "JsonMediumSerializerVisitor.h"
#include "model/Medium.h"
#include "persistence/MediaSerializationConfigs.h"

using Core::Model::Medium;

namespace Core::Persistence::Json {

class JsonMediaSerializer {
    std::unique_ptr<JsonMediumSerializerVisitor> mediumSerializer;

  public:
    explicit JsonMediaSerializer(
        MediaSerializationConfigs newConfigs = MediaSerializationConfigs{});

    [[nodiscard]] auto serialize(const std::vector<const Medium *> &media) const -> QJsonDocument;
};

}

#endif