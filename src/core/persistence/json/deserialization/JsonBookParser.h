#ifndef JSONBOOKPARSER_H
#define JSONBOOKPARSER_H

#include "JsonMediumParser.h"
#include "JsonParsingLib.h"
#include "model/Book.h"
#include "persistence/MediaSerializationConfigs.h"

using Core::Model::Book;

namespace Core::Persistence::Json {

class JsonBookParser : public JsonMediumParser {

    JsonParsingLib<Book> lib;

  public:
    ~JsonBookParser() override = default;
    JsonBookParser(const JsonBookParser &) = default;
    JsonBookParser(JsonBookParser &&) = default;
    auto operator=(const JsonBookParser &) -> JsonBookParser & = default;
    auto operator=(JsonBookParser &&) -> JsonBookParser & = default;

    JsonBookParser(MediaSerializationConfigs newConfigs = MediaSerializationConfigs{});

    [[nodiscard]] auto parse(const QJsonObject &bookObject, const QString &version) const
        -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>> override;
};

}

#endif