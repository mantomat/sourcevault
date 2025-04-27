#include "TestOptionalField.h"

#include "model/OptionalField.h"

#include <QTest>

using Core::Model::OptionalField;

void TestOptionalField::testHas_data() {
    QTest::addColumn<OptionalField<int>>("optionalField");
    QTest::addColumn<bool>("expected");

    OptionalField<int> field{};

    QTest::addRow("Newly created field should be unset") << field << false;

    field.set(0);
    QTest::addRow("Set field should have a value") << field << true;

    field.unset();
    QTest::addRow("Unset field should be unset") << field << false;
}
void TestOptionalField::testHas() {
    QFETCH(OptionalField<int>, optionalField);
    QFETCH(bool, expected);
    QCOMPARE(optionalField.has(), expected);
}

void TestOptionalField::testGet_data() {
    QTest::addColumn<OptionalField<int>>("optionalField");
    QTest::addColumn<std::optional<int>>("expected");

    OptionalField<int> field{};

    QTest::addRow("Newly created field should be unset")
        << field << std::optional<int>{std::nullopt};

    field.set(0);
    QTest::addRow("Set field should have the set value") << field << std::make_optional(0);

    field.unset();
    QTest::addRow("Unset field should be unset") << field << std::optional<int>{std::nullopt};
}
void TestOptionalField::testGet() {
    QFETCH(OptionalField<int>, optionalField);
    QFETCH(std::optional<int>, expected);
    QCOMPARE(optionalField.get(), expected);
}

void TestOptionalField::testSet_data() {
    QTest::addColumn<OptionalField<int>>("optionalField");
    QTest::addColumn<int>("expected");

    OptionalField<int> field{};

    field.set(100);
    QTest::addRow("First value (100)") << field << 100;

    field.set(0);
    QTest::addRow("Second value (0)") << field << 0;

    field.set(INT_MAX);
    QTest::addRow("Third value (INT_MAX)") << field << INT_MAX;
}
void TestOptionalField::testSet() {
    QFETCH(OptionalField<int>, optionalField);
    QFETCH(int, expected);
    QCOMPARE(optionalField.get(), expected);
}

void TestOptionalField::testUnset_data() {
    QTest::addColumn<OptionalField<int>>("optionalField");

    OptionalField<int> field{};

    QTest::addRow("Unset newly created field") << field;

    field.set(100);
    field.unset();
    QTest::addRow("Unset set field") << field;

    field.unset();
    QTest::addRow("Unset just-unset-field") << field;
}
void TestOptionalField::testUnset() {
    QFETCH(OptionalField<int>, optionalField);

    QCOMPARE(optionalField.has(), false);
    QCOMPARE(optionalField.get(), std::nullopt);
}