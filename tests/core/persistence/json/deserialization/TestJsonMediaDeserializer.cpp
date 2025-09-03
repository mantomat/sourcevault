#include "TestJsonMediaDeserializer.h"

#include "persistence/json/deserialization/JsonMediaDeserializer.h"
#include "persistence/json/deserialization/engine/JsonDeserializationError.h"
#include "persistence/json/deserialization/parsers/TestJsonArticleParser.h"
#include "persistence/json/deserialization/parsers/TestJsonBookParser.h"
#include "persistence/json/deserialization/parsers/TestJsonVideoParser.h"
#include "testutils/fixtures/JsonSerializationFixtures.h"

#include <QTest>

using Core::Persistence::Json::JsonDeserializationError;
using Core::Persistence::Json::JsonMediaDeserializer;

void TestJsonMediaDeserializer::testDeserialize_data() {
    QTest::addColumn<QJsonDocument>("document");
    QTest::addColumn<std::optional<JsonDeserializationError>>("expectedError");
    QTest::addColumn<std::optional<Library>>("expectedLibrary");

    JsonLibraryFixture fixture{};
    auto id{QUuid::createUuid().toString(QUuid::WithoutBraces)};

    QTest::addRow("Full library, three media (one per type)")
        << fixture.json << std::optional<JsonDeserializationError>{std::nullopt}
        << std::make_optional<Library>(fixture.library);
    {
        QJsonDocument empty;
        QTest::addRow("Null document")
            << empty
            << std::make_optional<JsonDeserializationError>(
                   JsonDeserializationError{.code = JsonDeserializationError::Code::InvalidJson,
                                            .errorLocation = empty,
                                            .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        QJsonDocument invalidRoot{QJsonArray{}};
        QTest::addRow("Invalid root type - array instead of object")
            << invalidRoot
            << std::make_optional<JsonDeserializationError>(
                   JsonDeserializationError{.code = JsonDeserializationError::Code::InvalidRootType,
                                            .errorLocation = invalidRoot,
                                            .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto noVersionObject{fixture.json.object()};
        noVersionObject.remove("version");
        QJsonDocument noVersion{noVersionObject};
        QTest::addRow("Missing version field")
            << noVersion
            << std::make_optional<JsonDeserializationError>(
                   JsonDeserializationError{.code = JsonDeserializationError::Code::MissingVersion,
                                            .errorLocation = noVersion.object(),
                                            .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto unknownVersionObject{fixture.json.object()};
        unknownVersionObject["version"] = "2.0";
        QJsonDocument unknownVersion{unknownVersionObject};
        QTest::addRow("Unknow version value")
            << unknownVersion
            << std::make_optional<JsonDeserializationError>(
                   JsonDeserializationError{.code = JsonDeserializationError::Code::UnknownVersion,
                                            .errorLocation = unknownVersion.object(),
                                            .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto noMediaObject{fixture.json.object()};
        noMediaObject.remove("media");
        QJsonDocument noMedia{noMediaObject};
        QTest::addRow("Missing media field")
            << noMedia
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::MissingMediaArray,
                   .errorLocation = noMedia.object(),
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto tooManyFieldsObject{fixture.json.object()};
        tooManyFieldsObject["oneOff"] = "I'm here!";
        QJsonDocument tooManyFields{tooManyFieldsObject};
        QTest::addRow("Too many fields in root object")
            << tooManyFields
            << std::make_optional<JsonDeserializationError>(
                   JsonDeserializationError{.code = JsonDeserializationError::Code::TooManyFields,
                                            .errorLocation = tooManyFields.object(),
                                            .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto nonObjectMediumObject{fixture.json.object()};
        nonObjectMediumObject["media"] = QJsonArray{nonObjectMediumObject["media"], "b"};
        QJsonDocument nonObjectMedium{nonObjectMediumObject};
        QTest::addRow("An entry of the media array is not an object")
            << nonObjectMedium
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::NonObjectMediaEntry,
                   .errorLocation = nonObjectMedium.object().value("media").toArray(),
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto missingMediumTypeObject{fixture.json.object()};
        const auto mediumMissingType{QJsonObject{{"id", id}, {"title", "t"}, {"favorite", false}}};
        missingMediumTypeObject["media"] = QJsonArray{mediumMissingType};
        QJsonDocument missingMediumType{missingMediumTypeObject};
        QTest::addRow("Missing type field in medium object")
            << missingMediumType
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::MissingMediumType,
                   .errorLocation = mediumMissingType,
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto unknownMediumTypeObject{fixture.json.object()};
        const auto mediumUnknownType{
            QJsonObject{{"type", 10}, {"id", id}, {"title", "t"}, {"favorite", false}}};
        unknownMediumTypeObject["media"] = QJsonArray{mediumUnknownType};
        QJsonDocument unknownMediumType{unknownMediumTypeObject};
        QTest::addRow("type field in medium object has wrong type")
            << unknownMediumType
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::UnknownMediumType,
                   .errorLocation = mediumUnknownType,
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto unknownMediumTypeObject{fixture.json.object()};
        const auto mediumUnknownType{
            QJsonObject{{"type", "hello!"}, {"id", id}, {"title", id}, {"favorite", false}}};
        unknownMediumTypeObject["media"] = QJsonArray{mediumUnknownType};
        QJsonDocument unknownMediumType{unknownMediumTypeObject};
        QTest::addRow("type field in medium object is unknown")
            << unknownMediumType
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::UnknownMediumType,
                   .errorLocation = mediumUnknownType,
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto rootObj{fixture.json.object()};
        const auto mediumObj{QJsonObject{{"type", "book"}, {"title", "t"}, {"favorite", false}}};
        rootObj["media"] = QJsonArray{mediumObj};
        QJsonDocument doc{rootObj};
        QTest::addRow("Missing id field in medium object")
            << doc
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::MissingRequiredField,
                   .errorLocation = mediumObj,
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto rootObj{fixture.json.object()};
        const auto mediumObj{
            QJsonObject{{"type", "book"}, {"id", 10}, {"title", "t"}, {"favorite", false}}};
        rootObj["media"] = QJsonArray{mediumObj};
        QJsonDocument doc{rootObj};
        QTest::addRow("Id field in medium object has wrong type")
            << doc
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::MissingRequiredField,
                   .errorLocation = mediumObj,
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto rootObj{fixture.json.object()};
        const auto mediumObj{QJsonObject{{"type", "book"}, {"id", id}, {"favorite", false}}};
        rootObj["media"] = QJsonArray{mediumObj};
        QJsonDocument doc{rootObj};
        QTest::addRow("Missing title field in medium entry")
            << doc
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::MissingRequiredField,
                   .errorLocation = mediumObj,
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto rootObj{fixture.json.object()};
        const auto mediumObj{
            QJsonObject{{"type", "book"}, {"title", 10}, {"id", id}, {"favorite", false}}};
        rootObj["media"] = QJsonArray{mediumObj};
        QJsonDocument doc{rootObj};
        QTest::addRow("title field has wrong type in medium entry")
            << doc
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::MissingRequiredField,
                   .errorLocation = mediumObj,
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto rootObj{fixture.json.object()};
        const auto mediumObj{
            QJsonObject{{"type", "book"}, {"title", ""}, {"id", id}, {"favorite", false}}};
        rootObj["media"] = QJsonArray{mediumObj};
        QJsonDocument doc{rootObj};
        QTest::addRow("title field is semantically invalid in medium entry")
            << doc
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::SemanticValidationFailed,
                   .errorLocation = mediumObj,
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto rootObj{fixture.json.object()};
        const auto mediumObj{QJsonObject{{"type", "book"}, {"title", "t"}, {"id", id}}};
        rootObj["media"] = QJsonArray{mediumObj};
        QJsonDocument doc{rootObj};
        QTest::addRow("favorite field is missing in medium entry")
            << doc
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::MissingRequiredField,
                   .errorLocation = mediumObj,
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
    {
        auto rootObj{fixture.json.object()};
        const auto mediumObj{
            QJsonObject{{"type", "book"}, {"title", "t"}, {"id", id}, {"favorite", "true"}}};
        rootObj["media"] = QJsonArray{mediumObj};
        QJsonDocument doc{rootObj};
        QTest::addRow("favorite field has wrong type in medium entry")
            << doc
            << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
                   .code = JsonDeserializationError::Code::MissingRequiredField,
                   .errorLocation = mediumObj,
                   .message = "not tested"})
            << std::optional<Library>(std::nullopt);
    }
}

void TestJsonMediaDeserializer::testDeserialize() {
    QFETCH(QJsonDocument, document);
    QFETCH(std::optional<JsonDeserializationError>, expectedError);
    QFETCH(std::optional<Library>, expectedLibrary);
    assert(expectedError.has_value() != expectedLibrary.has_value());

    JsonMediaDeserializer deserializer{};

    auto result{deserializer.deserialize(document)};

    if (expectedError.has_value()) {
        QVERIFY2(std::holds_alternative<JsonDeserializationError>(result),
                 "The parser should have returned an error, but it succeeded.");

        const auto &actualError = std::get<JsonDeserializationError>(result);
        QCOMPARE(actualError.code, expectedError.value().code);
        QCOMPARE(actualError.errorLocation, expectedError.value().errorLocation);
        return;
    }

    QVERIFY2(std::holds_alternative<Library>(result),
             "The parser should have succeeded, but it returned an error.");
    auto parsedLibrary = std::get<Library>(std::move(result));

    auto actualMedia{parsedLibrary.getMedia()};
    auto expectedMedia{parsedLibrary.getMedia()};

    QCOMPARE(actualMedia.size(), expectedMedia.size());

    std::ranges::sort(actualMedia);
    std::ranges::sort(expectedMedia);
    for (size_t i{0}; i < actualMedia.size(); i++) {
        if (const Book *expectedBook{dynamic_cast<const Book *>(expectedMedia[i])};
            expectedBook != nullptr) {
            const Book *actual{dynamic_cast<const Book *>(actualMedia[i])};

            QVERIFY(actual != nullptr);
            TestJsonBookParser::testEqualityNoDate(*actual, *expectedBook);
        } else if (const Article *expectedArticle{dynamic_cast<const Article *>(expectedMedia[i])};
                   expectedArticle != nullptr) {
            const Article *actual{dynamic_cast<const Article *>(actualMedia[i])};

            QVERIFY(actual != nullptr);
            TestJsonArticleParser::testEqualityNoDate(*actual, *expectedArticle);
        } else if (const Video *expectedVideo{dynamic_cast<const Video *>(expectedMedia[i])};
                   expectedVideo != nullptr) {
            const Video *actual{dynamic_cast<const Video *>(actualMedia[i])};

            QVERIFY(actual != nullptr);
            TestJsonVideoParser::testEqualityNoDate(*actual, *expectedVideo);
        }
    }
}