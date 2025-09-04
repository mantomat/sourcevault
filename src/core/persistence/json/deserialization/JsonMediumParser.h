#ifndef JSONMEDIUMPARSER_H
#define JSONMEDIUMPARSER_H

#include "JsonParsingLib.h"

#include <QJsonObject>

namespace Core::Persistence::Json {

class JsonMediumParser {
  public:
    virtual ~JsonMediumParser() = default;
    JsonMediumParser() = default;
    JsonMediumParser(const JsonMediumParser &) = default;
    JsonMediumParser(JsonMediumParser &&) = default;
    auto operator=(const JsonMediumParser &) -> JsonMediumParser & = default;
    auto operator=(JsonMediumParser &&) -> JsonMediumParser & = default;

    [[nodiscard]] virtual auto parse(const QJsonObject &obj, const QString &version) const
        -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>> = 0;
};

}

#endif