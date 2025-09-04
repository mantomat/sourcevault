#include "JsonVideoParser.h"

#include "model/Video.h"

using Core::Model::Video;

namespace Core::Persistence::Json {

JsonVideoParser::JsonVideoParser(MediaSerializationConfigs newConfigs)
    : lib{std::move(newConfigs)} {}

auto JsonVideoParser::parse(const QJsonObject &videoObject, const QString &version) const
    -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>> {
    // We only manage this version for now.
    assert(version == "1.0");

    using Lib = JsonParsingLib<Video>;

    auto result{lib.deserializeCommonFields(videoObject, version)};

    result = Lib::andThen(std::move(result), lib.optionalValidatedFieldParser<QUrl>(
                                                 videoObject, "videoUrl",
                                                 [](Video &v) -> auto & { return v.videoUrl(); }));
    result = Lib::andThen(std::move(result),
                          lib.optionalValidatedFieldParser<unsigned int>(
                              videoObject, "durationSeconds",
                              [](Video &v) -> auto & { return v.durationSeconds(); }));
    result = Lib::andThen(std::move(result), lib.optionalValidatedFieldParser<QDate>(
                                                 videoObject, "uploadDate", [](Video &v) -> auto & {
                                                     return v.uploadDate();
                                                 }));
    result =
        Lib::andThen(std::move(result), lib.optionalValidatedFieldParser<QUrl>(
                                            videoObject, "thumbnailUrl",
                                            [](Video &v) -> auto & { return v.thumbnailUrl(); }));

    if (auto *success = std::get_if<std::unique_ptr<Video>>(&result)) {
        return std::move(*success);
    }
    return std::get<JsonDeserializationError>(result);
}

}