#include "JsonArticleParser.h"

#include "model/Article.h"
#include "persistence/MediaSerializationConfigs.h"

using Core::Model::Article;

namespace Core::Persistence::Json {

JsonArticleParser::JsonArticleParser(MediaSerializationConfigs newConfigs)
    : configs{std::move(newConfigs)} {}

auto JsonArticleParser::parse(const QJsonObject &articleObject, const QString &version) const
    -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>> {
    // We only manage this version for now.
    assert(version == "1.0");

    QString title{articleObject.value("title").toString()};
    const QUuid id{articleObject.value("id").toString()};
    std::unique_ptr<Article> article{
        Article::make(std::move(title), id, QDate::currentDate()).value()};

    auto result{deserializeCommonFields<Article>(std::move(article), articleObject, version)};

    result = andThen<Article>(
        std::move(result),
        optionalValidatedFieldParser<QUrl, Article>(
            articleObject, "isbn", [](Article &a) -> auto & { return a.articleUrl(); }));
    result = andThen<Article>(
        std::move(result),
        optionalValidatedFieldParser<QString, Article>(
            articleObject, "sourceName", [](Article &a) -> auto & { return a.sourceName(); }));
    result = andThen<Article>(std::move(result),
                              optionalValidatedFieldParser<unsigned int, Article>(
                                  articleObject, "readTimeMinutes",
                                  [](Article &a) -> auto & { return a.readTimeMinutes(); }));
    result = andThen<Article>(std::move(result),
                              optionalValidatedFieldParser<QDate, Article>(
                                  articleObject, "publicationDate",
                                  [](Article &a) -> auto & { return a.publicationDate(); }));

    if (auto *success = std::get_if<std::unique_ptr<Article>>(&result)) {
        return std::move(*success);
    }
    return std::get<JsonDeserializationError>(result);
}
}