#include "TestMedium.h"

#include <QTest>

void TestMedium::testUserData() {
    ConcreteMedium medium;

    const QString notes{"Valid notes"};
    const std::set topics{QString{"Computer science"}, QString{"Philosophy"}};
    constexpr bool favorite{true};
    constexpr auto priority{Core::Model::MediumUserData::PriorityLevel::mid};

    medium.userData().notes().set(notes);
    medium.userData().topics().set(topics);
    medium.userData().favorite() = favorite;
    medium.userData().priority().set(priority);

    const ConcreteMedium constMedium{medium};
    QCOMPARE(constMedium.userData().notes().has(), true);
    QCOMPARE(constMedium.userData().topics().has(), true);
    QCOMPARE(constMedium.userData().favorite(), true);
    QCOMPARE(constMedium.userData().priority().has(), true);
    QCOMPARE(constMedium.userData().notes().get(), notes);
    QCOMPARE(constMedium.userData().topics().get(), topics);
    QCOMPARE(constMedium.userData().favorite(), favorite);
    QCOMPARE(constMedium.userData().priority().get(), priority);
}

void TestMedium::testDateAddedValidator_data() {
    QTest::addColumn<QDate>("candidate");
    QTest::addColumn<bool>("expected");

    QTest::addRow("A default constructed date should be invalid") << QDate{} << false;
    QTest::addRow("A defined date should be valid") << QDate{2025, 4, 22} << true;
}
void TestMedium::testDateAddedValidator() {
    QFETCH(QDate, candidate);
    QFETCH(bool, expected);
    QCOMPARE(ConcreteMedium::dateAddedValidator(candidate), expected);
}
void TestMedium::testDateAdded() {
    ConcreteMedium medium{};

    const QDate validDate{2025, 4, 22};
    medium.dateAdded().set(validDate);
    const ConcreteMedium constMediumValidDate{medium};
    QCOMPARE(constMediumValidDate.dateAdded().get(), std::make_optional(validDate));

    constexpr QDate invalidDate{};
    medium.dateAdded().set(invalidDate);
    const ConcreteMedium constMediumInvalidDate{medium};
    QCOMPARE(constMediumInvalidDate.dateAdded().get(), std::nullopt);
}

void TestMedium::testTitleValidator_data() {
    QTest::addColumn<QString>("candidate");
    QTest::addColumn<bool>("expected");

    QTest::addRow("An empty string should be invalid") << QString{} << false;
    QTest::addRow("Every non-empty string should be valid") << QString{"non-empty"} << true;
}
void TestMedium::testTitleValidator() {
    QFETCH(QString, candidate);
    QFETCH(bool, expected);
    QCOMPARE(ConcreteMedium::titleValidator(candidate), expected);
}
void TestMedium::testTitle() {
    ConcreteMedium medium{};

    const QString validTitle{"Norwegian Wood"};
    medium.title().set(validTitle);
    const ConcreteMedium constMediumValidTitle{medium};
    QCOMPARE(constMediumValidTitle.title().get(), std::make_optional(validTitle));

    const QString invalidTitle{};
    medium.title().set(invalidTitle);
    const ConcreteMedium constMediumInvalidTitle{medium};
    QCOMPARE(constMediumInvalidTitle.title().get(), std::nullopt);
}

void TestMedium::testAuthorValidator_data() {
    QTest::addColumn<QString>("candidate");
    QTest::addColumn<bool>("expected");

    QTest::addRow("An empty string should be invalid") << QString{} << false;
    QTest::addRow("Every non-empty string should be valid") << QString{"non-empty"} << true;
}
void TestMedium::testAuthorValidator() {
    QFETCH(QString, candidate);
    QFETCH(bool, expected);
    QCOMPARE(ConcreteMedium::authorValidator(candidate), expected);
}
void TestMedium::testAuthors() {
    ConcreteMedium medium{};

    const std::set validAuthors{QString{"Cormen"}, QString{"Skiena"}};
    medium.authors().set(validAuthors);
    const ConcreteMedium constMediumValidAuthors{medium};
    QCOMPARE(constMediumValidAuthors.authors().get(), std::make_optional(validAuthors));

    const std::set invalidAuthors{QString{}};
    medium.authors().set(invalidAuthors);
    const ConcreteMedium constMediumInvalidAuthors{medium};
    QCOMPARE(constMediumInvalidAuthors.authors().get(), std::nullopt);
}

void TestMedium::testLanguageValidator_data() {
    QTest::addColumn<QString>("candidate");
    QTest::addColumn<bool>("expected");

    QTest::addRow("An empty string should be invalid") << QString{} << false;
    QTest::addRow("Every non-empty string should be valid") << QString{"non-empty"} << true;
}
void TestMedium::testLanguageValidator() {
    QFETCH(QString, candidate);
    QFETCH(bool, expected);
    QCOMPARE(ConcreteMedium::languageValidator(candidate), expected);
}
void TestMedium::testLanguage() {
    ConcreteMedium medium{};

    const QString validLanguage{"EN"};
    medium.language().set(validLanguage);
    const ConcreteMedium constMediumValidLanguage{medium};
    QCOMPARE(constMediumValidLanguage.language().get(), std::make_optional(validLanguage));

    const QString invalidLanguage{};
    medium.language().set(invalidLanguage);
    const ConcreteMedium constMediumInvalidLanguage{medium};
    QCOMPARE(constMediumInvalidLanguage.language().get(), std::nullopt);
}