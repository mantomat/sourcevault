#ifndef JSONVIDEOPARSER_H
#define JSONVIDEOPARSER_H

#include "JsonMediumParser.h"
#include "JsonParsingLib.h"
#include "model/Video.h"
#include "persistence/MediaSerializationConfigs.h"

using Core::Model::Video;

namespace Core::Persistence::Json {

class JsonVideoParser : public JsonMediumParser {

    JsonParsingLib<Video> lib;

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