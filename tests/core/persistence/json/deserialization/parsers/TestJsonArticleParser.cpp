#include "TestJsonArticleParser.h"

#include "model/Article.h"
#include "persistence/MediaSerializationConfigs.h"
#include "persistence/json/deserialization/engine/JsonDeserializationError.h"
#include "persistence/json/deserialization/parsers/JsonArticleParser.h"
#include "persistence/json/serialization/JsonMediumSerializerVisitor.h"

#include <QTest>
#include <qjsonvalue.h>

using Core::Model::Article;
using Core::Persistence::MediaSerializationConfigs;
using Core::Persistence::Json::JsonArticleParser;
using Core::Persistence::Json::JsonDeserializationError;

// utils
namespace {

struct ArticleTestFixture {
    Article article{[] {
        Article newArticle =
            Article::create("Test Title", QUuid::createUuid(), QDate(2025, 1, 1)).value();
        newArticle.userData().favorite() = true;
        newArticle.authors().set({"Joan Westenberg"});
        newArticle.language().set("English");
        newArticle.userData().topics().set({"Emotions", "Personal Growth"});
        newArticle.userData().notes().set("Example notes...");
        newArticle.userData().priority().set(MediumUserData::PriorityLevel::mid);
        newArticle.articleUrl().set(QUrl{
            "https://www.joanwestenberg.com/p/how-we-traded-anxiety-for-apathy-d9fec4ba127f6971"});
        newArticle.sourceName().set("Westenber Blog");
        newArticle.readTimeMinutes().set(5);
        newArticle.publicationDate().set(QDate{2025, 8, 11});
        return newArticle;
    }()};

    QJsonObject json{[this] {
        QJsonObject newJson = {
            {"type", "article"},
            {"id", article.id().toString(QUuid::WithoutBraces)},
            {"title", article.title()},
            {"favorite", article.userData().favorite()},
            {"authors", QJsonArray{"Joan Westenberg"}},
            {"language", article.language().get().value()},
            {"topics", QJsonArray{"Emotions", "Personal Growth"}},
            {"notes", article.userData().notes().get().value()},
            {"priority", static_cast<int>(article.userData().priority().get().value())},
            {"articleUrl", article.articleUrl().get().value().toString()},
            {"sourceName", article.sourceName().get().value()},
            {"readTimeMinutes", static_cast<int>(article.readTimeMinutes().get().value())},
            {"publicationDate",
             article.publicationDate().get()->toString(MediaSerializationConfigs{}.dateFormat)},
        };
        return newJson;
    }()};
};

template <typename FieldType>
void testOptionalValidatedField(
    const QString &fieldName, const std::function<FieldType &(Article &)> &fieldGetter,
    const std::optional<QJsonValue> &semanticallyInvalidValue = std::nullopt) {
    ArticleTestFixture fixture{};

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

    ArticleTestFixture fixture{};

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
        const auto actualError = std::get<JsonDeserializationError>(parseResult);
        QCOMPARE(actualError.code, expectedError.value().code);
        QCOMPARE(actualError.errorLocation, expectedError.value().errorLocation);
        return;
    }

    QVERIFY2(std::holds_alternative<std::unique_ptr<const Medium>>(parseResult),
             "The parser should have succeeded, but it returned an error.");
    auto parsedMedium = std::get<std::unique_ptr<const Medium>>(std::move(parseResult));
    const auto *parsedArticle = dynamic_cast<const Article *>(parsedMedium.get());
    QVERIFY2(parsedArticle != nullptr, "The parsed medium is not an Article.");

    const auto &expected = expectedArticle.value();

    QCOMPARE(parsedArticle->id(), expected.id());
    QCOMPARE(parsedArticle->title(), expected.title());
    QCOMPARE(parsedArticle->userData().favorite(), expected.userData().favorite());
    QCOMPARE(parsedArticle->authors().get(), expected.authors().get());
    QCOMPARE(parsedArticle->language().get(), expected.language().get());
    QCOMPARE(parsedArticle->userData().topics().get(), expected.userData().topics().get());
    QCOMPARE(parsedArticle->userData().notes().get(), expected.userData().notes().get());
    QCOMPARE(parsedArticle->userData().priority().get(), expected.userData().priority().get());
    QCOMPARE(parsedArticle->articleUrl().get(), expected.articleUrl().get());
    QCOMPARE(parsedArticle->sourceName().get(), expected.sourceName().get());
    QCOMPARE(parsedArticle->readTimeMinutes().get(), expected.readTimeMinutes().get());
    QCOMPARE(parsedArticle->publicationDate().get(), expected.publicationDate().get());
}