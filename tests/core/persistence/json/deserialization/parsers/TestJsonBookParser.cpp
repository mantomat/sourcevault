#include "TestJsonBookParser.h"

#include "model/Book.h"
#include "model/MediumUserData.h"
#include "persistence/json/deserialization/engine/JsonDeserializationError.h"
#include "persistence/json/deserialization/parsers/JsonBookParser.h"
#include "testutils/fixtures/JsonSerializationFixtures.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTest>

using Core::Model::Book;
using Core::Model::MediumUserData;
using Core::Persistence::Json::JsonBookParser;
using Core::Persistence::Json::JsonDeserializationError;

// utils

namespace {

template <typename FieldType>
void testOptionalValidatedField(
    const QString &fieldName, const std::function<FieldType &(Book &)> &fieldGetter,
    const std::optional<QJsonValue> &semanticallyInvalidValue = std::nullopt) {
    BookJsonFixture fixture{};

    Book bWithNoField{fixture.book};
    fieldGetter(bWithNoField).unset();
    QJsonObject jWithNoField{fixture.json};
    jWithNoField.remove(fieldName);
    auto testCaseName{QString{"`%1` field is missing"}.arg(fieldName)};
    QTest::addRow("%s", testCaseName.toStdString().c_str())
        << jWithNoField << std::optional<JsonDeserializationError>{std::nullopt}
        << std::make_optional<Book>(bWithNoField);

    QJsonObject jWithWrongType{fixture.json};
    jWithWrongType[fieldName] = true;
    testCaseName = QString{"`%1` field has wrong type"}.arg(fieldName);
    QTest::addRow("%s", testCaseName.toStdString().c_str())
        << jWithWrongType
        << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
               .code = JsonDeserializationError::Code::WrongMediumFieldType,
               .errorLocation = jWithWrongType,
               .message = "not tested"})
        << std::optional<Book>{std::nullopt};

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
        << std::optional<Book>{std::nullopt};
}

}

void TestJsonBookParser::testParse_data() {
    QTest::addColumn<QJsonObject>("bookObject");
    // We can't use variants in this case, so we use two optionals, but they are mutually exclusive!
    QTest::addColumn<std::optional<JsonDeserializationError>>("expectedError");
    QTest::addColumn<std::optional<Book>>("expectedBook");

    BookJsonFixture fixture{};

    QTest::addRow("Full valid book")
        << fixture.json << std::optional<JsonDeserializationError>{std::nullopt}
        << std::make_optional<Book>(fixture.book);

    testOptionalValidatedField<ValidatedField<QString>>(
        "isbn", [](Book &b) -> auto & { return b.isbn(); }, "any non-isbn string is invalid");

    testOptionalValidatedField<ValidatedField<QString>>(
        "edition", [](Book &b) -> auto & { return b.edition(); }, "");

    testOptionalValidatedField<ValidatedField<QString>>(
        "publisher", [](Book &b) -> auto & { return b.publisher(); }, "");

    testOptionalValidatedField<ValidatedField<int>>(
        "yearPublished", [](Book &b) -> auto & { return b.yearPublished(); }, -100000);

    testOptionalValidatedField<ValidatedField<unsigned int>>(
        "pageNumber", [](Book &b) -> auto & { return b.pageNumber(); }, 0);

    testOptionalValidatedField<ValidatedField<QString>>(
        "description", [](Book &b) -> auto & { return b.description(); }, "");

    testOptionalValidatedField<ValidatedField<QUrl>>(
        "thumbnailUrl", [](Book &b) -> auto & { return b.thumbnailUrl(); },
        "https://m.media-amazon.com/images/I/71f6DceqZAL._SL1500_.gif");
}
void TestJsonBookParser::testParse() {
    QFETCH(QJsonObject, bookObject);
    QFETCH(std::optional<JsonDeserializationError>, expectedError);
    QFETCH(std::optional<Book>, expectedBook);
    assert(expectedError.has_value() != expectedBook.has_value());

    JsonBookParser parser;
    const QString version = "1.0";
    auto parseResult = parser.parse(bookObject, version);

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
    const auto *parsedBook = dynamic_cast<const Book *>(parsedMedium.get());
    QVERIFY2(parsedBook != nullptr, "The parsed medium is not of the right type.");

    testEqualityNoDate(*parsedBook, expectedBook.value());
}

void TestJsonBookParser::testEqualityNoDate(const Book &lhs, const Book &rhs) {
    QCOMPARE(lhs.id(), rhs.id());
    QCOMPARE(lhs.title(), rhs.title());
    QCOMPARE(lhs.userData().favorite(), rhs.userData().favorite());
    QCOMPARE(lhs.authors(), rhs.authors());
    QCOMPARE(lhs.language(), rhs.language());
    QCOMPARE(lhs.userData().topics(), rhs.userData().topics());
    QCOMPARE(lhs.userData().notes(), rhs.userData().notes());
    QCOMPARE(lhs.userData().priority(), rhs.userData().priority());
    QCOMPARE(lhs.isbn(), rhs.isbn());
    QCOMPARE(lhs.edition(), rhs.edition());
    QCOMPARE(lhs.publisher(), rhs.publisher());
    QCOMPARE(lhs.yearPublished(), rhs.yearPublished());
    QCOMPARE(lhs.pageNumber(), rhs.pageNumber());
    QCOMPARE(lhs.description(), rhs.description());
    QCOMPARE(lhs.thumbnailUrl(), rhs.thumbnailUrl());
}