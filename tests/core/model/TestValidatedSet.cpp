#include "TestValidatedSet.h"

#include "model/ValidatedSet.h"

#include <QTest>

using Core::Model::ValidatedSet;

void TestValidatedSet::testHas_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<bool>("expected");

    ValidatedSet<QString> set{[](const QString&) { return true; }};
    QTest::addRow("New set should be unset") << set << false;

    set.add(QString{"Valid value"});
    QTest::addRow("Set should have a value") << set << true;

    set.unset();
    QTest::addRow("Set should be unset") << set << false;
}
void TestValidatedSet::testHas() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(bool, expected);

    QCOMPARE(validatedSet.has(), expected);
}

void TestValidatedSet::testUnset_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");

    ValidatedSet<QString> set{[](const QString&) { return true; }};
    QTest::addRow("Unsetting a new set should do nothing") << set;

    set.add(QString{"Valid value"});
    QTest::addRow("Unsetting a non-empty set should remove the value") << set;
}
void TestValidatedSet::testUnset() {
    QFETCH(ValidatedSet<QString>, validatedSet);

    validatedSet.unset();
    QCOMPARE(validatedSet.has(), false);
    QCOMPARE(validatedSet.get(), std::nullopt);
}

void TestValidatedSet::testSetValid_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<std::set<QString>>("validValueToSet");

    const ValidatedSet<QString> set{[](const QString& str) { return !str.trimmed().isEmpty(); }};

    QTest::addRow("Perfectly valid data") << set << std::set{QString{"     a   \t"}};
    QTest::addRow("Perfectly valid data")
        << set << std::set{QString{"吉伊杰杰勒, 吾勒艾杰迪!"}, QString{"non-empty string"}};
}
void TestValidatedSet::testSetValid() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(std::set<QString>, validValueToSet);

    const bool res{validatedSet.set(validValueToSet)};

    QCOMPARE(res, true);
    QCOMPARE(validatedSet.has(), true);
    QCOMPARE(validatedSet.get(), validValueToSet);
}

void TestValidatedSet::testSetInvalid_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<std::set<QString>>("invalidValueToSet");

    ValidatedSet<QString> set{
        [](const QString& str) { return !str.trimmed().isEmpty() && str != QString{"invalid"}; }};

    QTest::addRow("Empty sets are invalid") << set << std::set<QString>{};
    QTest::addRow("Set made of invalid elements are invalid")
        << set << std::set{QString{" "}, QString{}, QString{"invalid"}};
    QTest::addRow("Just one invalid element invalidates the entire set")
        << set << std::set{QString{"Perfectly valid"}, QString{"   valid"}, QString{"invalid"}};
}
void TestValidatedSet::testSetInvalid() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(std::set<QString>, invalidValueToSet);

    const bool res{validatedSet.set(invalidValueToSet)};

    QCOMPARE(res, false);
    QCOMPARE(validatedSet.has(), false);
    QCOMPARE(validatedSet.get(), std::nullopt);
}

void TestValidatedSet::testSetWithMovedParameter_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<std::set<QString>>("validValueToSet");

    const ValidatedSet<QString> set{[](const QString&) { return true; }};

    QTest::addRow("Perfectly valid data")
        << set << std::set{QString{"     a   \t"}, QString{"Another one"}};
    QTest::addRow("Perfectly valid data")
        << set << std::set{QString{"吉伊杰杰勒, 吾勒艾杰迪!"}, QString{"non-empty string"}};
}
void TestValidatedSet::testSetWithMovedParameter() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(std::set<QString>, validValueToSet);

    std::set validValueToSetCopy{validValueToSet};
    const bool hasBeenSet{validatedSet.set(std::move(validValueToSetCopy))};

    QCOMPARE(hasBeenSet, true);
    QCOMPARE(validatedSet.has(), true);
    QCOMPARE(validatedSet.get(), validValueToSet);
}

void TestValidatedSet::testOverwriteValue_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<std::set<QString>>("firstValueToBeSet");
    QTest::addColumn<std::set<QString>>("secondValueToBeSet");

    const ValidatedSet<QString> set{[](const QString&) { return true; }};

    QTest::addRow("The first value should be completely overwritten by the second")
        << set << std::set{QString{"first"}, QString{"second"}}
        << std::set{QString{"third"}, QString{"fourth"}};
}
void TestValidatedSet::testOverwriteValue() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(std::set<QString>, firstValueToBeSet);
    QFETCH(std::set<QString>, secondValueToBeSet);

    bool res{validatedSet.set(firstValueToBeSet)};
    QCOMPARE(res, true);
    QCOMPARE(validatedSet.has(), true);
    QCOMPARE(validatedSet.get(), firstValueToBeSet);

    res = validatedSet.set(secondValueToBeSet);
    QCOMPARE(res, true);
    QCOMPARE(validatedSet.has(), true);
    QCOMPARE(validatedSet.get(), secondValueToBeSet);
}

void TestValidatedSet::testAddValid_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<std::set<QString>>("validElementsToAdd");

    const ValidatedSet<QString> set{[](const QString&) { return true; }};

    QTest::addRow("Three valid elements")
        << set << std::set{QString{"First"}, QString{"Second"}, QString{"Third"}};
    QTest::addRow("Perfectly valid data")
        << set << std::set{QString{"吉伊杰杰勒, 吾勒艾杰迪!"}, QString{"non-empty string"}};
}
void TestValidatedSet::testAddValid() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(std::set<QString>, validElementsToAdd);

    bool hasBeenAdded{false};
    std::set<QString> expected{};
    for (const auto& element : validElementsToAdd) {
        hasBeenAdded = validatedSet.add(element);
        expected.insert(element);

        QCOMPARE(hasBeenAdded, true);
        QCOMPARE(validatedSet.has(), true);
        QCOMPARE(validatedSet.get(), expected);
    }
}

void TestValidatedSet::testAddInvalid_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<QString>("invalidElementToAdd");

    const ValidatedSet<QString> set{[](const QString& el) { return !el.isEmpty(); }};

    QTest::addRow("Invalid elements shouldn't be added (empty string)") << set << QString{""};
    QTest::addRow("Invalid elements shouldn't be added (null string)") << set << QString{};
}
void TestValidatedSet::testAddInvalid() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(QString, invalidElementToAdd);

    const bool res{validatedSet.add(invalidElementToAdd)};

    QCOMPARE(res, false);
    QCOMPARE(validatedSet.has(), false);
    QCOMPARE(validatedSet.get(), std::nullopt);
}

void TestValidatedSet::testAddParameterTypeCoercion_data() {
    QTest::addColumn<ValidatedSet<double>>("validatedSet");
    QTest::addColumn<int>("validElementToAdd");

    const ValidatedSet<double> set{[](const double&) { return true; }};

    QTest::addRow("Valid element, coercion from int to double") << set << 200;
}
void TestValidatedSet::testAddParameterTypeCoercion() {
    QFETCH(ValidatedSet<double>, validatedSet);
    QFETCH(int, validElementToAdd);

    const bool res{validatedSet.add(validElementToAdd)};

    QCOMPARE(res, true);
    QCOMPARE(validatedSet.has(), true);
    QCOMPARE(validatedSet.get(), std::set{static_cast<double>(validElementToAdd)});
}

void TestValidatedSet::testAddWithMovedParameter_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<std::vector<QString>>("validElementsToAdd");

    const ValidatedSet<QString> set{[](const QString&) { return true; }};

    QTest::addRow("Three valid elements")
        << set << std::vector{QString{"First"}, QString{"Second"}, QString{"Third"}};
    QTest::addRow("Perfectly valid data")
        << set << std::vector{QString{"吉伊杰杰勒, 吾勒艾杰迪!"}, QString{"non-empty string"}};
}
void TestValidatedSet::testAddWithMovedParameter() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(std::vector<QString>, validElementsToAdd);

    bool hasBeenAdded{false};
    std::set<QString> expected{};
    for (QString& element : validElementsToAdd) {
        expected.insert(element);
        hasBeenAdded = validatedSet.add(std::move(element));

        QCOMPARE(hasBeenAdded, true);
        QCOMPARE(validatedSet.has(), true);
        QCOMPARE(validatedSet.get(), expected);
    }
}

void TestValidatedSet::testAddAlreadyPresent_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<QString>("presentElementToAdd");

    ValidatedSet<QString> set{[](const QString&) { return true; }};
    set.set(std::set{QString{"one"}, QString{"two"}, QString{"three"}});

    QTest::addRow("First already present element") << set << QString{"one"};
    QTest::addRow("Second already present element") << set << QString{"two"};
    QTest::addRow("First already present element") << set << QString{"three"};
}
void TestValidatedSet::testAddAlreadyPresent() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(QString, presentElementToAdd);

    const auto initialSet{validatedSet.get()};

    const bool hasBeenAdded{validatedSet.add(presentElementToAdd)};

    QCOMPARE(hasBeenAdded, false);
    QCOMPARE(validatedSet.has(), true);
    QCOMPARE(validatedSet.get(), initialSet);
}

void TestValidatedSet::testRemovePresent_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<QString>("presentElementToRemove");

    ValidatedSet<QString> set{[](const QString&) { return true; }};
    set.set(std::set{QString{"one"}, QString{"two"}, QString{"three"}});

    QTest::addRow("Remove first element") << set << QString{"one"};
    QTest::addRow("Remove element in the middle") << set << QString{"two"};
    QTest::addRow("Remove last element") << set << QString{"three"};
}
void TestValidatedSet::testRemovePresent() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(QString, presentElementToRemove);

    auto initialSet{validatedSet.get()};
    initialSet->erase(presentElementToRemove);

    validatedSet.remove(presentElementToRemove);

    QCOMPARE(validatedSet.get(), initialSet);
}

void TestValidatedSet::testRemoveAbsent_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<QString>("absentElementToRemove");

    ValidatedSet<QString> set{[](const QString&) { return true; }};
    set.set(std::set{QString{"one"}, QString{"two"}, QString{"three"}});

    QTest::addRow("Remove fourth element") << set << QString{"four"};
    QTest::addRow("Remove fifth in the middle") << set << QString{"five"};
    QTest::addRow("Remove sixth element") << set << QString{"six"};
}
void TestValidatedSet::testRemoveAbsent() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(QString, absentElementToRemove);

    const auto initialSet{validatedSet.get()};

    validatedSet.remove(absentElementToRemove);

    QCOMPARE(validatedSet.get(), initialSet);
}

void TestValidatedSet::testGetValid_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<std::set<QString>>("expected");

    ValidatedSet<QString> set{[](const QString&) { return true; }};
    std::set expected{QString{"valid one"}, QString{"valid two"}};
    set.set(expected);

    QTest::addRow("Get should always return the value when a value is present") << set << expected;
}

void TestValidatedSet::testGetValid() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(std::set<QString>, expected);

    QCOMPARE(validatedSet.get(), expected);
}

void TestValidatedSet::testGetUnset_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");

    ValidatedSet<QString> set{[](const QString&) { return true; }};

    QTest::addRow("Get should always return nullopt when no value is present") << set;
}
void TestValidatedSet::testGetUnset() {
    QFETCH(ValidatedSet<QString>, validatedSet);

    QCOMPARE(validatedSet.get(), std::nullopt);
}

void TestValidatedSet::testSetUnsetTransition() {
    ValidatedSet<QString> set{[](const QString&) { return true; }};

    const std::set firstSet{QString{"1.1"}, QString{"1.2"}};
    bool res{set.set(firstSet)};
    QCOMPARE(res, true);
    QCOMPARE(set.has(), true);
    QCOMPARE(set.get(), firstSet);

    set.unset();
    QCOMPARE(set.has(), false);
    QCOMPARE(set.get(), std::nullopt);

    res = set.set(firstSet);
    QCOMPARE(res, true);
    QCOMPARE(set.has(), true);
    QCOMPARE(set.get(), firstSet);
}