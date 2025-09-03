#include "TestJsonArticleParser.h"

#include "model/Article.h"
#include "persistence/MediaSerializationConfigs.h"
#include "persistence/json/deserialization/engine/JsonDeserializationError.h"
#include "persistence/json/deserialization/parsers/JsonArticleParser.h"
#include "persistence/json/serialization/JsonMediumSerializerVisitor.h"
#include "testutils/fixtures/JsonSerializationFixtures.h"

#include <QTest>

using Core::Model::Article;
using Core::Persistence::MediaSerializationConfigs;
using Core::Persistence::Json::JsonArticleParser;
using Core::Persistence::Json::JsonDeserializationError;

namespace {
template <typename FieldType>
void testOptionalValidatedField(
    const QString &fieldName, const std::function<FieldType &(Article &)> &fieldGetter,
    const std::optional<QJsonValue> &semanticallyInvalidValue = std::nullopt) {
    ArticleJsonFixture fixture{};

    Article aWithNoField{fixture.article};
    fieldGetter(aWithNoField).unset();
    QJsonObject jWithNoField{fixture.json};
    jWithNoField.remove(fieldName);
    auto testCaseName{QString{"`%1` field is missing"}.arg(fieldName)};
    QTest::addRow("%s", testCaseName.toStdString().c_str())
        << jWithNoField << std::optional<JsonDeserializationError>{std::nullopt}
        << std::make_optional<Article>(aWithNoField);

    QJsonObject jWithWrongType{fixture.json};
    jWithWrongType[fieldName] = true;
    testCaseName = QString{"`%1` field has wrong type"}.arg(fieldName);
    QTest::addRow("%s", testCaseName.toStdString().c_str())
        << jWithWrongType
        << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
               .code = JsonDeserializationError::Code::WrongMediumFieldType,
               .errorLocation = jWithWrongType,
               .message = "not tested"})
        << std::optional<Article>{std::nullopt};

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
        << std::optional<Article>{std::nullopt};
}
}

void TestJsonArticleParser::testParse_data() {
    QTest::addColumn<QJsonObject>("articleObject");
    // We can't use variants in this case, so we use two optionals, but they are mutually exclusive!
    QTest::addColumn<std::optional<JsonDeserializationError>>("expectedError");
    QTest::addColumn<std::optional<Article>>("expectedArticle");

    ArticleJsonFixture fixture{};

    QTest::addRow("Full valid article")
        << fixture.json << std::optional<JsonDeserializationError>{std::nullopt}
        << std::make_optional<Article>(fixture.article);

    testOptionalValidatedField<ValidatedField<QUrl>>(
        "articleUrl", [](Article &a) -> auto & { return a.articleUrl(); }, "htps:/helo.comaaoaua");

    testOptionalValidatedField<ValidatedField<QString>>(
        "sourceName", [](Article &a) -> auto & { return a.sourceName(); }, "");

    testOptionalValidatedField<ValidatedField<unsigned int>>(
        "readTimeMinutes", [](Article &a) -> auto & { return a.readTimeMinutes(); }, 0);

    testOptionalValidatedField<ValidatedField<QDate>>(
        "publicationDate", [](Article &a) -> auto & { return a.publicationDate(); });
}
void TestJsonArticleParser::testParse() {
    QFETCH(QJsonObject, articleObject);
    QFETCH(std::optional<JsonDeserializationError>, expectedError);
    QFETCH(std::optional<Article>, expectedArticle);
    assert(expectedError.has_value() != expectedArticle.has_value());

    JsonArticleParser parser;
    const QString version = "1.0";
    auto parseResult = parser.parse(articleObject, version);

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
    const auto *parsedArticle = dynamic_cast<const Article *>(parsedMedium.get());
    QVERIFY2(parsedArticle != nullptr, "The parsed medium is not an Article.");

    testEqualityNoDate(*parsedArticle, expectedArticle.value());
}

void TestJsonArticleParser::testEqualityNoDate(const Article &lhs, const Article &rhs) {
    QCOMPARE(lhs.id(), rhs.id());
    QCOMPARE(lhs.title(), rhs.title());
    QCOMPARE(lhs.userData().favorite(), rhs.userData().favorite());
    QCOMPARE(lhs.authors(), rhs.authors());
    QCOMPARE(lhs.language(), rhs.language());
    QCOMPARE(lhs.userData().topics(), rhs.userData().topics());
    QCOMPARE(lhs.userData().notes(), rhs.userData().notes());
    QCOMPARE(lhs.userData().priority(), rhs.userData().priority());
    QCOMPARE(lhs.articleUrl(), rhs.articleUrl());
    QCOMPARE(lhs.sourceName(), rhs.sourceName());
    QCOMPARE(lhs.readTimeMinutes(), rhs.readTimeMinutes());
    QCOMPARE(lhs.publicationDate(), rhs.publicationDate());
}