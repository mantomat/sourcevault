#ifndef JSONVIDEOPARSER_H
#define JSONVIDEOPARSER_H

#include "JsonMediumParser.h"
#include "persistence/MediaSerializationConfigs.h"

namespace Core::Persistence::Json {

class JsonVideoParser : public JsonMediumParser {

    MediaSerializationConfigs configs;

  public:
    ~JsonVideoParser() override = default;
    JsonVideoParser(const JsonVideoParser &) = default;
    JsonVideoParser(JsonVideoParser &&) = default;
    auto operator=(const JsonVideoParser &) -> JsonVideoParser & = default;
    auto operator=(JsonVideoParser &&) -> JsonVideoParser & = default;

    explicit JsonVideoParser(MediaSerializationConfigs newConfigs = MediaSerializationConfigs{});

    [[nodiscard]] auto parse(const QJsonObject &videoObject, const QString &version) const
        -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>> override;
};

}

#endif