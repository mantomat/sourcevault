#ifndef JSONARTICLEPARSER_H
#define JSONARTICLEPARSER_H

#include "persistence/json/JsonMediumParser.h"

namespace Core::Persistence::Json {

class JsonArticleParser : public JsonMediumParser {

    MediaSerializationConfigs configs;

  public:
    ~JsonArticleParser() override = default;
    JsonArticleParser(const JsonArticleParser &) = default;
    JsonArticleParser(JsonArticleParser &&) = default;
    auto operator=(const JsonArticleParser &) -> JsonArticleParser & = default;
    auto operator=(JsonArticleParser &&) -> JsonArticleParser & = default;

    explicit JsonArticleParser(MediaSerializationConfigs configs);

    [[nodiscard]] auto parse(const QJsonObject &articleObject, const QString &version) const
        -> std::variant<DeserializationError, std::unique_ptr<const Medium>> override;
};

}

#endif