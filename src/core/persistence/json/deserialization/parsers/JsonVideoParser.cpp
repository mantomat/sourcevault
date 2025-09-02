#include "JsonVideoParser.h"

#include "model/Video.h"

using Core::Model::Video;

namespace Core::Persistence::Json {

JsonVideoParser::JsonVideoParser(MediaSerializationConfigs newConfigs)
    : configs{std::move(newConfigs)} {}

auto JsonVideoParser::parse(const QJsonObject &videoObject, const QString &version) const
    -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>> {
    // We only manage this version for now.
    assert(version == "1.0");

    QString title{videoObject.value("title").toString()};
    const QUuid id{videoObject.value("id").toString()};
    std::unique_ptr<Video> video{Video::make(std::move(title), id, QDate::currentDate()).value()};

    auto result{deserializeCommonFields<Video>(std::move(video), videoObject, version)};

    result = andThen<Video>(std::move(result), optionalValidatedFieldParser<QUrl, Video>(
                                                   videoObject, "videoUrl", [](Video &v) -> auto & {
                                                       return v.videoUrl();
                                                   }));
    result = andThen<Video>(std::move(result),
                            optionalValidatedFieldParser<unsigned int, Video>(
                                videoObject, "durationSeconds",
                                [](Video &v) -> auto & { return v.durationSeconds(); }));
    result =
        andThen<Video>(std::move(result), optionalValidatedFieldParser<QDate, Video>(
                                              videoObject, "uploadDate",
                                              [](Video &v) -> auto & { return v.uploadDate(); }));
    result =
        andThen<Video>(std::move(result), optionalValidatedFieldParser<QUrl, Video>(
                                              videoObject, "thumbnailUrl",
                                              [](Video &v) -> auto & { return v.thumbnailUrl(); }));

    if (auto *success = std::get_if<std::unique_ptr<Video>>(&result)) {
        return std::move(*success);
    }
    return std::get<JsonDeserializationError>(result);
}

}