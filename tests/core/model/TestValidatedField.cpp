#include "TestValidatedField.h"

#include "model/ValidatedField.h"

#include <QTest>

using Core::Model::ValidatedField;

void TestValidatedField::testHas_data() {
    QTest::addColumn<ValidatedField<QString>>("validatedField");
    QTest::addColumn<bool>("expected");

    ValidatedField<QString> field{[](const QString&) { return true; }};
    QTest::addRow("New field should be unset") << field << false;

    field.set(QString{"Valid value"});
    QTest::addRow("Field should be set") << field << true;

    field.unset();
    QTest::addRow("Field should be unset") << field << false;
}
void TestValidatedField::testHas() {
    QFETCH(ValidatedField<QString>, validatedField);
    QFETCH(bool, expected);

    QCOMPARE(validatedField.has(), expected);
}

void TestValidatedField::testUnset_data() {
    QTest::addColumn<ValidatedField<QString>>("validatedField");

    ValidatedField<QString> field{[](const QString&) { return true; }};
    QTest::addRow("Unsetting a new field should do nothing") << field;

    field.set(QString{"Valid value"});
    QTest::addRow("Unsetting a set field should unset the field") << field;
}
void TestValidatedField::testUnset() {
    QFETCH(ValidatedField<QString>, validatedField);

    validatedField.unset();
    QCOMPARE(validatedField.has(), false);
    QCOMPARE(validatedField.get(), std::nullopt);
}

void TestValidatedField::testSetValid_data() {
    QTest::addColumn<ValidatedField<QString>>("validatedField");
    QTest::addColumn<QString>("validValueToSet");

    const ValidatedField<QString> field{[](const QString&) { return true; }};

    QTest::addRow("Perfectly valid data") << field << QString{"     a   \t"};
    QTest::addRow("Perfectly valid data") << field << QString{"吉伊杰杰勒, 吾勒艾杰迪!"};
}
void TestValidatedField::testSetValid() {
    QFETCH(ValidatedField<QString>, validatedField);
    QFETCH(QString, validValueToSet);

    const bool res{validatedField.set(validValueToSet)};

    QCOMPARE(res, true);
    QCOMPARE(validatedField.has(), true);
    QCOMPARE(validatedField.get(), validValueToSet);
}

void TestValidatedField::testSetInvalid_data() {
    QTest::addColumn<ValidatedField<QString>>("validatedField");
    QTest::addColumn<QString>("invalidValueToSet");

    ValidatedField<QString> field{
        [](const QString& str) { return !str.trimmed().isEmpty() && str != QString{"invalid"}; }};

    QTest::addRow("Invalid empty string") << field << QString{"        "};
    QTest::addRow("Invalid tabs string") << field << QString{"     \n   \t"};
    QTest::addRow("Invalid string") << field << QString{"invalid"};

    field.set(QString{"Valid value"});
    QTest::addRow("Trying to set an invalid value should unset the previous one")
        << field << QString{};
}
void TestValidatedField::testSetInvalid() {
    QFETCH(ValidatedField<QString>, validatedField);
    QFETCH(QString, invalidValueToSet);

    const bool res{validatedField.set(invalidValueToSet)};

    QCOMPARE(res, false);
    QCOMPARE(validatedField.has(), false);
    QCOMPARE(validatedField.get(), std::nullopt);
}

void TestValidatedField::testSetParameterTypeCoercion_data() {
    QTest::addColumn<ValidatedField<double>>("validatedField");
    QTest::addColumn<int>("validValueToSet");

    const ValidatedField<double> field{[](const double&) { return true; }};

    QTest::addRow("Perfectly valid data, coercion from int to double") << field << 200;
}
void TestValidatedField::testSetParameterTypeCoercion() {
    QFETCH(ValidatedField<double>, validatedField);
    QFETCH(int, validValueToSet);

    const bool res{validatedField.set(validValueToSet)};

    QCOMPARE(res, true);
    QCOMPARE(validatedField.has(), true);
    QCOMPARE(validatedField.get(), validValueToSet);
}

void TestValidatedField::testSetWithMovedParameter_data() {
    QTest::addColumn<ValidatedField<QString>>("validatedField");
    QTest::addColumn<QString>("validValueToSet");

    const ValidatedField<QString> field{[](const QString&) { return true; }};

    QTest::addRow("Perfectly valid data") << field << QString{"     a   \t"};
    QTest::addRow("Perfectly valid data") << field << QString{"吉伊杰杰勒, 吾勒艾杰迪!"};
}
void TestValidatedField::testSetWithMovedParameter() {
    QFETCH(ValidatedField<QString>, validatedField);
    QFETCH(QString, validValueToSet);
    QString validValueToSetCopy{validValueToSet};

    const bool res = validatedField.set(std::move(validValueToSet));

    QCOMPARE(res, true);
    QCOMPARE(validatedField.has(), true);
    QCOMPARE(validatedField.get(), validValueToSetCopy);
}

void TestValidatedField::testOverwriteValue_data() {
    QTest::addColumn<ValidatedField<QString>>("validatedField");
    QTest::addColumn<QString>("firstValueToBeSet");
    QTest::addColumn<QString>("secondValueToBeSet");

    const ValidatedField<QString> field{[](const QString&) { return true; }};

    QTest::addRow("The first value should be completely overwritten by the second")
        << field << QString{"first"} << QString{"second"};
}
void TestValidatedField::testOverwriteValue() {
    QFETCH(ValidatedField<QString>, validatedField);
    QFETCH(QString, firstValueToBeSet);
    QFETCH(QString, secondValueToBeSet);

    bool res{validatedField.set(firstValueToBeSet)};
    QCOMPARE(res, true);
    QCOMPARE(validatedField.has(), true);
    QCOMPARE(validatedField.get(), firstValueToBeSet);

    res = validatedField.set(secondValueToBeSet);
    QCOMPARE(res, true);
    QCOMPARE(validatedField.has(), true);
    QCOMPARE(validatedField.get(), secondValueToBeSet);
}

void TestValidatedField::testGetValid_data() {
    QTest::addColumn<ValidatedField<QString>>("validatedField");
    QTest::addColumn<QString>("expected");

    ValidatedField<QString> field{[](const QString&) { return true; }};
    QString expected{"Valid value"};
    field.set(expected);

    QTest::addRow("Get should always return the value when a value is present")
        << field << expected;
}
void TestValidatedField::testGetValid() {
    QFETCH(ValidatedField<QString>, validatedField);
    QFETCH(QString, expected);

    QCOMPARE(validatedField.get(), expected);
}

void TestValidatedField::testGetUnset_data() {
    QTest::addColumn<ValidatedField<QString>>("validatedField");

    ValidatedField<QString> field{[](const QString&) { return true; }};

    QTest::addRow("Get should always return nullopt when no value is present") << field;
}
void TestValidatedField::testGetUnset() {
    QFETCH(ValidatedField<QString>, validatedField);

    QCOMPARE(validatedField.get(), std::nullopt);
}

void TestValidatedField::testSetUnsetTransition() {
    ValidatedField<QString> field{[](const QString&) { return true; }};

    const char* firstValue{
        "Btw, this is a const char* which can't be tested with parametrized tests in Qt"};
    bool res{field.set(firstValue)};
    QCOMPARE(res, true);
    QCOMPARE(field.has(), true);
    QCOMPARE(field.get(), QString{firstValue});

    field.unset();
    QCOMPARE(field.has(), false);
    QCOMPARE(field.get(), std::nullopt);

    res = field.set(firstValue);
    QCOMPARE(res, true);
    QCOMPARE(field.has(), true);
    QCOMPARE(field.get(), QString{firstValue});
}