#include "TestJsonVideoParser.h"

#include "model/MediumUserData.h"
#include "model/Video.h"
#include "persistence/MediaSerializationConfigs.h"
#include "persistence/json/deserialization/engine/JsonDeserializationError.h"
#include "persistence/json/deserialization/parsers/JsonVideoParser.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTest>

using Core::Model::MediumUserData;
using Core::Model::Video;
using Core::Persistence::Json::JsonDeserializationError;
using Core::Persistence::Json::JsonVideoParser;

namespace {

struct VideoTestFixture {
    Video video{[] {
        Video newVideo{Video::create("Test Title", QUuid::createUuid(), QDate(2025, 1, 1)).value()};
        newVideo.userData().favorite() = false;
        newVideo.authors().set({"Paolo Coletti"});
        newVideo.language().set("Italian");
        newVideo.userData().topics().set({"Finance", "Education"});
        newVideo.userData().notes().set("Example notes...");
        newVideo.userData().priority().set(MediumUserData::PriorityLevel::max);
        newVideo.videoUrl().set(QUrl{
            "https://www.youtube.com/watch?v=cSlr2I1GGiw&list=PLfuzpc-H8qcdyEb5rpgYhN2P7cJGR1i9h"});
        newVideo.durationSeconds().set(516);
        newVideo.uploadDate().set(QDate{2023, 8, 13});
        newVideo.thumbnailUrl().set(
            QUrl{"https://i.ytimg.com/vi/cSlr2I1GGiw/"
                 "hqdefault.jpg?sqp=-oaymwEcCPYBEIoBSFXyq4qpAw4IARUAAIhCGAFwAcABBg==&rs="
                 "AOn4CLAyPvvwnV5Kt15d_r8EvE72ZEJHfg"});
        return newVideo;
    }()};

    QJsonObject json{[this] {
        QJsonObject newJson{
            {"type", "video"},
            {"id", video.id().toString(QUuid::WithoutBraces)},
            {"title", video.title()},
            {"favorite", video.userData().favorite()},
            {"authors", QJsonArray{"Paolo Coletti"}},
            {"language", video.language().get().value()},
            {"topics", QJsonArray{"Finance", "Education"}},
            {"notes", video.userData().notes().get().value()},
            {"priority", static_cast<int>(video.userData().priority().get().value())},
            {"videoUrl", video.videoUrl().get().value().toString()},
            {"durationSeconds", static_cast<int>(video.durationSeconds().get().value())},
            {"uploadDate", video.uploadDate().get().value().toString(
                               Core::Persistence::MediaSerializationConfigs{}.dateFormat)},
            {"thumbnailUrl", video.thumbnailUrl().get().value().toString()},
        };
        return newJson;
    }()};
};

template <typename FieldType>
void testOptionalValidatedField(
    const QString &fieldName, const std::function<FieldType &(Video &)> &fieldGetter,
    const std::optional<QJsonValue> &semanticallyInvalidValue = std::nullopt) {
    VideoTestFixture fixture{};

    Video vWithNoField{fixture.video};
    fieldGetter(vWithNoField).unset();
    QJsonObject jWithNoField{fixture.json};
    jWithNoField.remove(fieldName);
    auto testCaseName{QString{"`%1` field is missing"}.arg(fieldName)};
    QTest::addRow("%s", testCaseName.toStdString().c_str())
        << jWithNoField << std::optional<JsonDeserializationError>{std::nullopt}
        << std::make_optional<Video>(vWithNoField);

    QJsonObject jWithWrongType{fixture.json};
    jWithWrongType[fieldName] = true;
    testCaseName = QString{"`%1` field has wrong type"}.arg(fieldName);
    QTest::addRow("%s", testCaseName.toStdString().c_str())
        << jWithWrongType
        << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
               .code = JsonDeserializationError::Code::WrongMediumFieldType,
               .errorLocation = jWithWrongType,
               .message = "not tested"})
        << std::optional<Video>{std::nullopt};

    if (!semanticallyInvalidValue.has_value()) {
        return;
    }

    QJsonObject jWithSemanticError{fixture.json};
    jWithSemanticError[fieldName] = semanticallyInvalidValue.value();
    testCaseName = QString{"`%1` field is semantically invalid"}.arg(fieldName);
    QTest::addRow("%s", testCaseName.toStdString().c_str())
        << jWithSemanticError
        << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
               .code = JsonDeserializationError::Code::SemanticValidationFailed,
               .errorLocation = jWithSemanticError,
               .message = "not tested"})
        << std::optional<Video>{std::nullopt};
}

}

void TestJsonVideoParser::testParse_data() {
    QTest::addColumn<QJsonObject>("videoObject");
    QTest::addColumn<std::optional<JsonDeserializationError>>("expectedError");
    QTest::addColumn<std::optional<Video>>("expectedVideo");

    VideoTestFixture fixture{};

    QTest::addRow("Full valid video")
        << fixture.json << std::optional<JsonDeserializationError>{std::nullopt}
        << std::make_optional<Video>(fixture.video);

    testOptionalValidatedField<ValidatedField<QUrl>>(
        "videoUrl", [](Video &v) -> auto & { return v.videoUrl(); },
        "https://github.com/mantomat/eigencheck/tree/main/qna");

    testOptionalValidatedField<ValidatedField<unsigned int>>(
        "durationSeconds", [](Video &v) -> auto & { return v.durationSeconds(); }, 0);

    testOptionalValidatedField<ValidatedField<QDate>>(
        "uploadDate", [](Video &v) -> auto & { return v.uploadDate(); },
        QDate{1970, 1, 1}.toString(Core::Persistence::MediaSerializationConfigs{}.dateFormat));

    testOptionalValidatedField<ValidatedField<QDate>>(
        "thumbnailUrl", [](Video &v) -> auto & { return v.uploadDate(); },
        QUrl{"https://i.ytimg.com/vi/cSlr2I1GGiw/hqdefault.psd"}.toString());
}
void TestJsonVideoParser::testParse() {
    QFETCH(QJsonObject, videoObject);
    QFETCH(std::optional<JsonDeserializationError>, expectedError);
    QFETCH(std::optional<Video>, expectedVideo);
    assert(expectedError.has_value() != expectedVideo.has_value());

    JsonVideoParser parser;
    const QString version = "1.0";
    auto parseResult = parser.parse(videoObject, version);

    if (expectedError.has_value()) {
        QVERIFY2(std::holds_alternative<JsonDeserializationError>(parseResult),
                 "The parser should have returned an error, but it succeeded.");
        const auto actualError = std::get<JsonDeserializationError>(parseResult);
        QCOMPARE(actualError.code, expectedError.value().code);
        QCOMPARE(actualError.errorLocation, expectedError.value().errorLocation);
        return;
    }

    QVERIFY2(std::holds_alternative<std::unique_ptr<const Medium>>(parseResult),
             "The parser should have succeeded, but it returned an error.");
    auto parsedMedium = std::get<std::unique_ptr<const Medium>>(std::move(parseResult));
    const auto *parsedVideo = dynamic_cast<const Video *>(parsedMedium.get());
    QVERIFY2(parsedVideo != nullptr, "The parsed medium is not of the right type.");

    const auto &expected = expectedVideo.value();

    QCOMPARE(parsedVideo->id(), expected.id());
    QCOMPARE(parsedVideo->title(), expected.title());
    QCOMPARE(parsedVideo->userData().favorite(), expected.userData().favorite());
    QCOMPARE(parsedVideo->authors().get(), expected.authors().get());
    QCOMPARE(parsedVideo->language().get(), expected.language().get());
    QCOMPARE(parsedVideo->userData().topics().get(), expected.userData().topics().get());
    QCOMPARE(parsedVideo->userData().notes().get(), expected.userData().notes().get());
    QCOMPARE(parsedVideo->userData().priority().get(), expected.userData().priority().get());
    QCOMPARE(parsedVideo->videoUrl().get(), expected.videoUrl().get());
    QCOMPARE(parsedVideo->durationSeconds().get(), expected.durationSeconds().get());
    QCOMPARE(parsedVideo->uploadDate().get(), expected.uploadDate().get());
    QCOMPARE(parsedVideo->thumbnailUrl().get(), expected.thumbnailUrl().get());
}