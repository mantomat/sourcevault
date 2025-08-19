#include "TestSort.h"

#include <QTest>

void TestSort::testConstructorAndIsAscending() {
    const ConcreteSort asc{};
    QCOMPARE(asc.isAscending(), true);

    const ConcreteSort desc{false};
    QCOMPARE(desc.isAscending(), false);
}

void TestSort::testClone() {
    const auto original{std::make_unique<ConcreteSort>(false)};

    const auto clone{original->clone()};

    QVERIFY(original != clone);
    QVERIFY(dynamic_cast<const ConcreteSort *>(clone.get()) != nullptr);
    QCOMPARE(original->isAscending(), clone->isAscending());
}