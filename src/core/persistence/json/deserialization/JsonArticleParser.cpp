#include "JsonArticleParser.h"

#include "model/Article.h"
#include "persistence/MediaSerializationConfigs.h"

using Core::Model::Article;

namespace Core::Persistence::Json {

JsonArticleParser::JsonArticleParser(MediaSerializationConfigs newConfigs)
    : lib{std::move(newConfigs)} {}

auto JsonArticleParser::parse(const QJsonObject &articleObject, const QString &version) const
    -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>> {
    // We only manage this version for now.
    assert(version == "1.0");

    using Lib = JsonParsingLib<Article>;

    auto result{lib.deserializeCommonFields(articleObject, version)};
    result =
        Lib::andThen(std::move(result), lib.optionalValidatedFieldParser<QUrl>(
                                            articleObject, "articleUrl",
                                            [](Article &a) -> auto & { return a.articleUrl(); }));
    result =
        Lib::andThen(std::move(result), lib.optionalValidatedFieldParser<QString>(
                                            articleObject, "sourceName",
                                            [](Article &a) -> auto & { return a.sourceName(); }));
    result = Lib::andThen(std::move(result),
                          lib.optionalValidatedFieldParser<unsigned int>(
                              articleObject, "readTimeMinutes",
                              [](Article &a) -> auto & { return a.readTimeMinutes(); }));
    result = Lib::andThen(std::move(result),
                          lib.optionalValidatedFieldParser<QDate>(
                              articleObject, "publicationDate",
                              [](Article &a) -> auto & { return a.publicationDate(); }));

    if (auto *success = std::get_if<std::unique_ptr<Article>>(&result)) {
        return std::move(*success);
    }
    return std::get<JsonDeserializationError>(result);
}
}