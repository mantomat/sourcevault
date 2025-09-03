#ifndef JSONARTICLEPARSER_H
#define JSONARTICLEPARSER_H

#include "JsonMediumParser.h"
#include "persistence/MediaSerializationConfigs.h"

namespace Core::Persistence::Json {

class JsonArticleParser : public JsonMediumParser {

    MediaSerializationConfigs configs;

  public:
    ~JsonArticleParser() override = default;
    JsonArticleParser(const JsonArticleParser &) = default;
    JsonArticleParser(JsonArticleParser &&) = default;
    auto operator=(const JsonArticleParser &) -> JsonArticleParser & = default;
    auto operator=(JsonArticleParser &&) -> JsonArticleParser & = default;

    explicit JsonArticleParser(MediaSerializationConfigs configs = MediaSerializationConfigs{});

    [[nodiscard]] auto parse(const QJsonObject &articleObject, const QString &version) const
        -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>> override;
};

}

#endif