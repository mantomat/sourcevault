#include "TestJsonVideoParser.h"

#include "model/MediumUserData.h"
#include "model/Video.h"
#include "persistence/MediaSerializationConfigs.h"
#include "persistence/json/deserialization/engine/JsonDeserializationError.h"
#include "persistence/json/deserialization/parsers/JsonVideoParser.h"
#include "testutils/fixtures/JsonSerializationFixtures.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTest>

using Core::Model::MediumUserData;
using Core::Model::Video;
using Core::Persistence::Json::JsonDeserializationError;
using Core::Persistence::Json::JsonVideoParser;

namespace {
template <typename FieldType>
void testOptionalValidatedField(
    const QString &fieldName, const std::function<FieldType &(Video &)> &fieldGetter,
    const std::optional<QJsonValue> &semanticallyInvalidValue = std::nullopt) {
    VideoJsonFixture fixture{};

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

    VideoJsonFixture fixture{};

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

    testOptionalValidatedField<ValidatedField<QUrl>>(
        "thumbnailUrl", [](Video &v) -> auto & { return v.thumbnailUrl(); },
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
        const auto &actualError = std::get<JsonDeserializationError>(parseResult);
        QCOMPARE(actualError.code, expectedError.value().code);
        QCOMPARE(actualError.errorLocation, expectedError.value().errorLocation);
        return;
    }

    QVERIFY2(std::holds_alternative<std::unique_ptr<const Medium>>(parseResult),
             "The parser should have succeeded, but it returned an error.");
    auto parsedMedium = std::get<std::unique_ptr<const Medium>>(std::move(parseResult));
    const auto *parsedVideo = dynamic_cast<const Video *>(parsedMedium.get());
    QVERIFY2(parsedVideo != nullptr, "The parsed medium is not of the right type.");

    testEqualityNoDate(*parsedVideo, expectedVideo.value());
}

void TestJsonVideoParser::testEqualityNoDate(const Video &lhs, const Video &rhs) {
    QCOMPARE(lhs.id(), rhs.id());
    QCOMPARE(lhs.title(), rhs.title());
    QCOMPARE(lhs.userData().favorite(), rhs.userData().favorite());
    QCOMPARE(lhs.authors(), rhs.authors());
    QCOMPARE(lhs.language(), rhs.language());
    QCOMPARE(lhs.userData().topics(), rhs.userData().topics());
    QCOMPARE(lhs.userData().notes(), rhs.userData().notes());
    QCOMPARE(lhs.userData().priority(), rhs.userData().priority());
    QCOMPARE(lhs.videoUrl(), rhs.videoUrl());
    QCOMPARE(lhs.durationSeconds(), rhs.durationSeconds());
    QCOMPARE(lhs.uploadDate(), rhs.uploadDate());
    QCOMPARE(lhs.thumbnailUrl(), rhs.thumbnailUrl());
}