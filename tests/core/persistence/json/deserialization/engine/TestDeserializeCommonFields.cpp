#include "TestDeserializeCommonFields.h"

#include "mocks/MockConcreteMedium.h"
#include "model/MediumUserData.h"
#include "persistence/json/deserialization/engine/JsonDeserializationError.h"
#include "persistence/json/deserialization/engine/JsonDeserializationUtils.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTest>
#include <variant>

using Core::Model::MediumUserData;
using Core::Persistence::Json::deserializeCommonFields;
using Core::Persistence::Json::JsonDeserializationError;

struct MediumTextFixture {
    MockConcreteMedium medium{[] {
        MockConcreteMedium newMedium;
        newMedium.userData().favorite() = true;
        newMedium.authors().set({"Joan Westenberg"});
        newMedium.language().set("English");
        newMedium.userData().topics().set({"Emotions", "Personal Growth"});
        newMedium.userData().notes().set("Example notes...");
        newMedium.userData().priority().set(MediumUserData::PriorityLevel::mid);
        return newMedium;
    }()};

    QJsonObject json{[this] {
        QJsonObject newJson = {
            {"type", "unused"},
            {"id", medium.id().toString(QUuid::WithoutBraces)},
            {"title", medium.title()},
            {"favorite", medium.userData().favorite()},
            {"authors", QJsonArray{"Joan Westenberg"}},
            {"language", medium.language().get().value()},
            {"topics", QJsonArray{"Emotions", "Personal Growth"}},
            {"notes", medium.userData().notes().get().value()},
            {"priority", static_cast<int>(medium.userData().priority().get().value())},
        };
        return newJson;
    }()};
};

namespace {

template <typename FieldType>
void testOptionalValidatedField(
    const QString &fieldName, const std::function<FieldType &(MockConcreteMedium &)> &fieldGetter,
    const std::optional<QJsonValue> &semanticallyInvalidValue = std::nullopt) {
    MediumTextFixture fixture{};

    MockConcreteMedium mWithNoField{fixture.medium};
    fieldGetter(mWithNoField).unset();
    QJsonObject jWithNoField{fixture.json};
    jWithNoField.remove(fieldName);
    auto testCaseName{QString{"`%1` field is missing"}.arg(fieldName)};
    QTest::addRow("%s", testCaseName.toStdString().c_str())
        << jWithNoField << std::optional<JsonDeserializationError>{std::nullopt}
        << std::make_optional<MockConcreteMedium>(mWithNoField);

    QJsonObject jWithWrongType{fixture.json};
    jWithWrongType[fieldName] = true;
    testCaseName = QString{"`%1` field has wrong type"}.arg(fieldName);
    QTest::addRow("%s", testCaseName.toStdString().c_str())
        << jWithWrongType
        << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
               .code = JsonDeserializationError::Code::WrongMediumFieldType,
               .errorLocation = jWithWrongType,
               .message = "not tested"})
        << std::optional<MockConcreteMedium>{std::nullopt};

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
        << std::optional<MockConcreteMedium>{std::nullopt};
}

template <typename FieldType>
void testOptionalValidatedSet(const QString &fieldName,
                              const std::function<FieldType &(MockConcreteMedium &)> &fieldGetter,
                              const QJsonValue &semanticallyInvalidValue) {

    testOptionalValidatedField(fieldName, fieldGetter,
                               std::make_optional(semanticallyInvalidValue));

    MediumTextFixture fixture{};
    QJsonObject o{fixture.json};
    o[fieldName] = QJsonArray{"a", "b", 12};
    auto testCaseName{
        QString{"`%1` field is an array but it has a non-string element"}.arg(fieldName)};
    QTest::addRow("%s", testCaseName.toStdString().c_str())
        << o
        << std::make_optional<JsonDeserializationError>(JsonDeserializationError{
               .code = JsonDeserializationError::Code::WrongMediumFieldType,
               .errorLocation = o,
               .message = "not tested"})
        << std::optional<MockConcreteMedium>{std::nullopt};
}

}

void TestDeserializeCommonFields::testDeserializeCommonFields_data() {
    QTest::addColumn<QJsonObject>("mediumObject");
    // We can't use variants in this case, so we use two optionals, but they are mutually exclusive!
    QTest::addColumn<std::optional<JsonDeserializationError>>("expectedError");
    QTest::addColumn<std::optional<MockConcreteMedium>>("expectedMedium");

    MediumTextFixture fixture{};

    QTest::addRow("Full valid medium")
        << fixture.json << std::optional<JsonDeserializationError>{std::nullopt}
        << std::make_optional<MockConcreteMedium>(fixture.medium);

    testOptionalValidatedSet<ValidatedSet<QString>>(
        "authors", [](MockConcreteMedium &m) -> auto & { return m.authors(); },
        QJsonArray{"author1", "", "author3"});

    testOptionalValidatedField<ValidatedField<QString>>(
        "language", [](MockConcreteMedium &m) -> auto & { return m.language(); }, "");

    testOptionalValidatedSet<ValidatedSet<QString>>(
        "topics", [](MockConcreteMedium &m) -> auto & { return m.userData().topics(); },
        QJsonArray{"topic1", "topic2", ""});

    testOptionalValidatedField<ValidatedField<QString>>(
        "notes", [](MockConcreteMedium &m) -> auto & { return m.userData().notes(); }, "");

    testOptionalValidatedField<ValidatedField<MediumUserData::PriorityLevel>>(
        "priority", [](MockConcreteMedium &m) -> auto & { return m.userData().priority(); });
}
void TestDeserializeCommonFields::testDeserializeCommonFields() {
    QFETCH(QJsonObject, mediumObject);
    QFETCH(std::optional<JsonDeserializationError>, expectedError);
    QFETCH(std::optional<MockConcreteMedium>, expectedMedium);
    assert(expectedError.has_value() != expectedMedium.has_value());

    const QString version = "1.0";
    auto parseResult = deserializeCommonFields<MockConcreteMedium>(mediumObject, version);

    if (expectedError.has_value()) {
        QVERIFY2(std::holds_alternative<JsonDeserializationError>(parseResult),
                 "The parser should have returned an error, but it succeeded.");
        const auto actualError = std::get<JsonDeserializationError>(parseResult);
        QCOMPARE(actualError.code, expectedError.value().code);
        QCOMPARE(actualError.errorLocation, expectedError.value().errorLocation);
        return;
    }

    QVERIFY2(std::holds_alternative<std::unique_ptr<MockConcreteMedium>>(parseResult),
             "The parser should have succeeded, but it returned an error.");
    auto parsedMedium = std::get<std::unique_ptr<MockConcreteMedium>>(std::move(parseResult));
    const auto *parseMedium = dynamic_cast<const MockConcreteMedium *>(parsedMedium.get());
    QVERIFY2(parseMedium != nullptr, "The parsed medium is not of the right type.");

    const auto &expected = expectedMedium.value();

    QCOMPARE(parseMedium->id(), expected.id());
    QCOMPARE(parseMedium->title(), expected.title());
    QCOMPARE(parseMedium->userData().favorite(), expected.userData().favorite());
    QCOMPARE(parseMedium->authors().get(), expected.authors().get());
    QCOMPARE(parseMedium->language().get(), expected.language().get());
    QCOMPARE(parseMedium->userData().topics().get(), expected.userData().topics().get());
    QCOMPARE(parseMedium->userData().notes().get(), expected.userData().notes().get());
    QCOMPARE(parseMedium->userData().priority().get(), expected.userData().priority().get());
}