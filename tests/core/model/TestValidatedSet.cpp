#include "TestValidatedSet.h"

#include "model/ValidatedSet.h"

#include <QTest>

using Core::Model::ValidatedSet;

void TestValidatedSet::testEqualsOperator_data() {
    QTest::addColumn<ValidatedSet<int>>("set1");
    QTest::addColumn<ValidatedSet<int>>("set2");
    QTest::addColumn<bool>("shouldBeEqual");

    {
        ValidatedSet<int> field{[](auto) { return true; }};
        QTest::addRow("Same data, same validator => ValidatedSets are equal")
            << field << field << true;
    }
    {
        ValidatedSet<int> positiveSet{[](int data) { return data > 0; }};
        positiveSet.set({1, 3});
        ValidatedSet<int> oddSet{[](int data) { return data % 2 == 1; }};
        oddSet.set({1, 3});
        QTest::addRow("Same data, different validator => ValidatedSets are still equal")
            << positiveSet << oddSet << true;
    }
    {
        ValidatedSet<int> positiveSet{[](int data) { return data > 0; }};
        positiveSet.add(1);
        ValidatedSet<int> positiveSet2{positiveSet};
        positiveSet.add(2);
        QTest::addRow("Different data, same validator => ValidatedSets are not equal")
            << positiveSet << positiveSet2 << false;
    }
    {
        ValidatedSet<int> evenSet{[](int data) { return data % 2 == 0; }};
        evenSet.add(2);
        ValidatedSet<int> oddSet{[](int data) { return data % 2 == 1; }};
        oddSet.add(1);
        QTest::addRow("Different data, different validator => ValidatedSets are not equal")
            << evenSet << oddSet << false;
    }
}
void TestValidatedSet::testEqualsOperator() {
    QFETCH(ValidatedSet<int>, set1);
    QFETCH(ValidatedSet<int>, set2);
    QFETCH(bool, shouldBeEqual);

    QCOMPARE(set1 == set2, shouldBeEqual);
}

void TestValidatedSet::testHas_data() {
    QTest::addColumn<ValidatedSet<QString>>("validatedSet");
    QTest::addColumn<bool>("shouldBeSet");

    {
        ValidatedSet<QString> set{[](const QString&) { return true; }};
        QTest::addRow("New set should be unset") << set << false;
    }
    {
        ValidatedSet<QString> set{[](const QString&) { return true; }};
        set.add(QString{"Valid value"});
        QTest::addRow("Set should have a value") << set << true;
    }
}
void TestValidatedSet::testHas() {
    QFETCH(ValidatedSet<QString>, validatedSet);
    QFETCH(bool, shouldBeSet);

    QCOMPARE(validatedSet.has(), shouldBeSet);
}

void TestValidatedSet::testGet_data() {
    using OptionalSet = std::optional<std::set<int>>;

    QTest::addColumn<ValidatedSet<int>>("validatedSet");
    QTest::addColumn<OptionalSet>("expectedData");

    {
        ValidatedSet<int> set{[](auto) { return true; }};
        QTest::addRow("An empty set returns nullopt") << set << OptionalSet{std::nullopt};
    }
    {
        ValidatedSet<int> set{[](auto) { return true; }};
        const std::set values{1, 2, 3};
        set.set(values);
        QTest::addRow("A non-empty set returns its data") << set << OptionalSet{values};
    }
}
void TestValidatedSet::testGet() {
    QFETCH(ValidatedSet<int>, validatedSet);
    QFETCH(std::optional<std::set<int>>, expectedData);

    QCOMPARE(validatedSet.get(), expectedData);
}

void TestValidatedSet::testSet_data() {
    QTest::addColumn<ValidatedSet<int>>("set");
    QTest::addColumn<std::set<int>>("valueToSet");
    QTest::addColumn<bool>("shouldSet");

    ValidatedSet<int> positiveSet{[](auto d) { return d > 0; }};
    positiveSet.set({1, 2, 3});

    QTest::addRow("A set containing all valid elements is valid")
        << positiveSet << std::set{2, 3, 4} << true;
    QTest::addRow("An empty set is invalid and it unsets the set")
        << positiveSet << std::set<int>{} << false;
    QTest::addRow("A set containing an invalid element is invalid and triggers an unset")
        << positiveSet << std::set{4, -1, 3} << false;
    QTest::addRow("A set containing all invalid elements is invalid and triggers an unset")
        << positiveSet << std::set{-1, -2, -3} << false;
}
void TestValidatedSet::testSet() {
    QFETCH(ValidatedSet<int>, set);
    QFETCH(std::set<int>, valueToSet);
    QFETCH(bool, shouldSet);

    const bool wasSet{set.set(valueToSet)};

    QCOMPARE(wasSet, shouldSet);
    QCOMPARE(set.get(), shouldSet ? std::make_optional(valueToSet) : std::nullopt);
}

void TestValidatedSet::testAdd_data() {
    QTest::addColumn<ValidatedSet<int>>("set");
    QTest::addColumn<int>("valueToAdd");
    QTest::addColumn<bool>("shouldAdd");

    {
        ValidatedSet<int> positiveSet{[](auto d) { return d > 0; }};
        QTest::addRow("Trying to add an invalid value does nothing and returns false")
            << positiveSet << -1 << false;
    }
    {
        ValidatedSet<int> positiveSet{[](auto d) { return d > 0; }};
        positiveSet.set({1});
        QTest::addRow("Trying to add a valid, present value does nothing and returns false")
            << positiveSet << 1 << false;
    }
    {
        ValidatedSet<int> positiveSet{[](auto d) { return d > 0; }};
        QTest::addRow("Trying to add a valid, absent value returns true")
            << positiveSet << 1 << true;
    }
}
void TestValidatedSet::testAdd() {
    QFETCH(ValidatedSet<int>, set);
    QFETCH(int, valueToAdd);
    QFETCH(bool, shouldAdd);

    const size_t previousSize{set.has() ? set.get().value().size() : 0};

    const bool wasAdded{set.add(valueToAdd)};

    QCOMPARE(wasAdded, shouldAdd);

    const size_t currentSize{set.has() ? set.get().value().size() : 0};
    QCOMPARE(currentSize, shouldAdd ? previousSize + 1 : previousSize);

    if (shouldAdd) {
        QVERIFY(set.has());
        QVERIFY(std::ranges::any_of(set.get().value(), [=](int d) { return d == valueToAdd; }));
    }
}

void TestValidatedSet::testRemove_data() {
    QTest::addColumn<ValidatedSet<int>>("validatedSet");
    QTest::addColumn<int>("elementToRemove");
    QTest::addColumn<bool>("shouldRemove");

    {
        ValidatedSet<int> set{[](auto) { return true; }};
        QTest::addRow("Trying to remove an absent element does nothing and returns false")
            << set << 1 << false;
    }
    {
        ValidatedSet<int> set{[](auto) { return true; }};
        set.set({1, 2, 3});
        QTest::addRow("Removing a present element returns true") << set << 1 << true;
    }
}
void TestValidatedSet::testRemove() {
    QFETCH(ValidatedSet<int>, validatedSet);
    QFETCH(int, elementToRemove);
    QFETCH(bool, shouldRemove);

    const size_t previousSize{validatedSet.has() ? validatedSet.get().value().size() : 0};

    const bool wasRemoved{validatedSet.remove(elementToRemove)};

    QCOMPARE(wasRemoved, shouldRemove);

    if (shouldRemove) {
        const size_t currentSize{validatedSet.has() ? validatedSet.get().value().size() : 0};
        QCOMPARE(currentSize, previousSize - 1);
    }

    if (validatedSet.has()) {
        QVERIFY(std::ranges::none_of(validatedSet.get().value(),
                                     [=](int d) { return d == elementToRemove; }));
    }
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
