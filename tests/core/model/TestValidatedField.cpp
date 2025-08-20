#include "TestValidatedField.h"

#include "model/ValidatedField.h"

#include <QTest>

using Core::Model::ValidatedField;

void TestValidatedField::testEqualsOperator_data() {
    QTest::addColumn<ValidatedField<int>>("field1");
    QTest::addColumn<ValidatedField<int>>("field2");
    QTest::addColumn<bool>("shouldBeEqual");

    {
        ValidatedField<int> field{[](auto) { return true; }};
        QTest::addRow("Same data, same validator => ValidatedFields are equal")
            << field << field << true;
    }
    {
        ValidatedField<int> positiveField{[](int data) { return data > 0; }};
        positiveField.set(1);
        ValidatedField<int> oddField{[](int data) { return data % 2 == 1; }};
        oddField.set(1);
        QTest::addRow("Same data, different validator => ValidatedFields are still equal")
            << positiveField << oddField << true;
    }
    {
        ValidatedField<int> positiveField1{[](int data) { return data > 0; }};
        positiveField1.set(1);
        ValidatedField<int> positiveField2{positiveField1};
        positiveField1.set(2);
        QTest::addRow("Different data, same validator => ValidatedFields are not equal")
            << positiveField1 << positiveField2 << false;
    }
    {
        ValidatedField<int> evenField{[](int data) { return data % 2 == 0; }};
        evenField.set(2);
        ValidatedField<int> oddField{[](int data) { return data % 2 == 1; }};
        oddField.set(1);
        QTest::addRow("Different data, different validator => ValidatedFields are not equal")
            << evenField << oddField << false;
    }
}
void TestValidatedField::testEqualsOperator() {
    QFETCH(ValidatedField<int>, field1);
    QFETCH(ValidatedField<int>, field2);
    QFETCH(bool, shouldBeEqual);

    QCOMPARE(field1 == field2, shouldBeEqual);
}

void TestValidatedField::testHas_data() {
    QTest::addColumn<ValidatedField<QString>>("validatedField");
    QTest::addColumn<bool>("shouldBeSet");

    {
        ValidatedField<QString> field{[](const QString &) { return true; }};
        QTest::addRow("New field is unset") << field << false;
    }
    {
        ValidatedField<QString> field{[](const QString &) { return true; }};
        field.set("Valid value");
        QTest::addRow("Field is set after setting") << field << true;
    }
}
void TestValidatedField::testHas() {
    QFETCH(ValidatedField<QString>, validatedField);
    QFETCH(bool, shouldBeSet);

    QCOMPARE(validatedField.has(), shouldBeSet);
}

void TestValidatedField::testGet_data() {
    QTest::addColumn<ValidatedField<int>>("field");
    QTest::addColumn<std::optional<int>>("expectedData");

    {
        const ValidatedField<int> field{[](auto) { return true; }};
        QTest::addRow("Empty field returns nullopt") << field << std::optional<int>{std::nullopt};
    }
    {
        ValidatedField<int> field{[](auto) { return true; }};
        constexpr int value{10};
        field.set(value);
        QTest::addRow("Empty field returns nullopt") << field << std::make_optional(value);
    }
}
void TestValidatedField::testGet() {
    QFETCH(ValidatedField<int>, field);
    QFETCH(std::optional<int>, expectedData);

    QCOMPARE(field.get(), expectedData);
}

void TestValidatedField::testSet_data() {
    QTest::addColumn<ValidatedField<int>>("field");
    QTest::addColumn<int>("valueToSet");
    QTest::addColumn<bool>("shouldSet");

    ValidatedField<int> positiveField{[](auto d) { return d > 0; }};
    positiveField.set(2);

    QTest::addRow("Positive integer should be set") << positiveField << 1 << true;
    QTest::addRow("Negative integer shouldn't be set, and the field should be unset")
        << positiveField << -1 << false;
}

void TestValidatedField::testSet() {
    QFETCH(ValidatedField<int>, field);
    QFETCH(int, valueToSet);
    QFETCH(bool, shouldSet);

    const bool wasSet{field.set(valueToSet)};

    QCOMPARE(wasSet, shouldSet);
    QCOMPARE(field.get(), shouldSet ? std::make_optional(valueToSet) : std::nullopt);
}

void TestValidatedField::testUnset_data() {
    QTest::addColumn<ValidatedField<QString>>("validatedField");

    ValidatedField<QString> field{[](const QString &) { return true; }};
    QTest::addRow("Unsetting an unset field should do nothing") << field;

    field.set(QString{"Valid value"});
    QTest::addRow("Unsetting a set field should unset the field") << field;
}
void TestValidatedField::testUnset() {
    QFETCH(ValidatedField<QString>, validatedField);

    validatedField.unset();
    QCOMPARE(validatedField.has(), false);
    QCOMPARE(validatedField.get(), std::nullopt);
}