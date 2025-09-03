#include "TestJsonBookParser.h"

#include "model/Book.h"
#include "model/MediumUserData.h"
#include "persistence/json/deserialization/engine/JsonDeserializationError.h"
#include "persistence/json/deserialization/parsers/JsonBookParser.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTest>

using Core::Model::Book;
using Core::Model::MediumUserData;
using Core::Persistence::Json::JsonBookParser;
using Core::Persistence::Json::JsonDeserializationError;

// utils

namespace {

struct BookTestFixture {
    Book book{[] {
        Book newBook{Book::create("Test Title", QUuid::createUuid(), QDate(2025, 1, 1)).value()};
        newBook.userData().favorite() = true;
        newBook.authors().set({"Daniel Kahneman"});
        newBook.language().set("English");
        newBook.userData().topics().set({"Psychology", "Economics"});
        newBook.userData().notes().set("Example notes...");
        newBook.userData().priority().set(MediumUserData::PriorityLevel::high);
        newBook.isbn().set("978-0141033570");
        newBook.edition().set("First edition");
        newBook.publisher().set("Penguin Psychology");
        newBook.yearPublished().set(2012);
        newBook.pageNumber().set(610);
        newBook.description().set("One of the most influential books of the 21st century: ...");
        newBook.thumbnailUrl().set(
            QUrl{"https://m.media-amazon.com/images/I/71f6DceqZAL._SL1500_.jpg"});
        return newBook;
    }()};

    QJsonObject json{[this] {
        QJsonObject newJson{
            {"type", "book"},
            {"id", book.id().toString(QUuid::WithoutBraces)},
            {"title", book.title()},
            {"favorite", book.userData().favorite()},
            {"authors", QJsonArray{"Daniel Kahneman"}},
            {"language", book.language().get().value()},
            {"topics", QJsonArray{"Psychology", "Economics"}},
            {"notes", book.userData().notes().get().value()},
            {"priority", static_cast<int>(book.userData().priority().get().value())},
            {"isbn", book.isbn().get().value()},
            {"edition", book.edition().get().value()},
            {"publisher", book.publisher().get().value()},
            {"yearPublished", book.yearPublished().get().value()},
            {"pageNumber", static_cast<int>(book.pageNumber().get().value())},
            {"description", book.description().get().value()},
            {"thumbnailUrl", book.thumbnailUrl().get().value().toString()},
        };
        return newJson;
    }()};
};

template <typename FieldType>
void testOptionalValidatedField(
    const QString &fieldName, const std::function<FieldType &(Book &)> &fieldGetter,
    const std::optional<QJsonValue> &semanticallyInvalidValue = std::nullopt) {
    BookTestFixture fixture{};

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

    BookTestFixture fixture{};

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
        const auto actualError = std::get<JsonDeserializationError>(parseResult);
        QCOMPARE(actualError.code, expectedError.value().code);
        QCOMPARE(actualError.errorLocation, expectedError.value().errorLocation);
        return;
    }

    QVERIFY2(std::holds_alternative<std::unique_ptr<const Medium>>(parseResult),
             "The parser should have succeeded, but it returned an error.");
    auto parsedMedium = std::get<std::unique_ptr<const Medium>>(std::move(parseResult));
    const auto *parsedBook = dynamic_cast<const Book *>(parsedMedium.get());
    QVERIFY2(parsedBook != nullptr, "The parsed medium is not of the right type.");

    const auto &expected = expectedBook.value();

    QCOMPARE(parsedBook->id(), expected.id());
    QCOMPARE(parsedBook->title(), expected.title());
    QCOMPARE(parsedBook->userData().favorite(), expected.userData().favorite());
    QCOMPARE(parsedBook->authors().get(), expected.authors().get());
    QCOMPARE(parsedBook->language().get(), expected.language().get());
    QCOMPARE(parsedBook->userData().topics().get(), expected.userData().topics().get());
    QCOMPARE(parsedBook->userData().notes().get(), expected.userData().notes().get());
    QCOMPARE(parsedBook->userData().priority().get(), expected.userData().priority().get());
    QCOMPARE(parsedBook->isbn().get(), expected.isbn().get());
    QCOMPARE(parsedBook->edition().get(), expected.edition().get());
    QCOMPARE(parsedBook->publisher().get(), expected.publisher().get());
    QCOMPARE(parsedBook->yearPublished().get(), expected.yearPublished().get());
    QCOMPARE(parsedBook->pageNumber().get(), expected.pageNumber().get());
    QCOMPARE(parsedBook->description().get(), expected.description().get());
    QCOMPARE(parsedBook->thumbnailUrl().get(), expected.thumbnailUrl().get());
}