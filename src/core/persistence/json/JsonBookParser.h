#ifndef JSONBOOKPARSER_H
#define JSONBOOKPARSER_H

#include "JsonDeserializationUtils.h"
#include "JsonMediumParser.h"
#include "persistence/MediaSerializationConfigs.h"

namespace Core::Persistence::Json {

class JsonBookParser : public JsonMediumParser {

    MediaSerializationConfigs configs;

  public:
    ~JsonBookParser() override = default;
    JsonBookParser(const JsonBookParser &) = default;
    JsonBookParser(JsonBookParser &&) = default;
    auto operator=(const JsonBookParser &) -> JsonBookParser & = default;
    auto operator=(JsonBookParser &&) -> JsonBookParser & = default;

    JsonBookParser(MediaSerializationConfigs newConfigs = MediaSerializationConfigs{});

    [[nodiscard]] auto parse(const QJsonObject &bookObject, const QString &version) const
        -> std::variant<DeserializationError, std::unique_ptr<const Medium>> override;
};

}

#endif