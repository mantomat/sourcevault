#include "TestMedium.h"

#include "TestValidatedField.h"
#include "TestValidatedSet.h"

#include <QTest>

using Core::Model::Medium;

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

void TestMedium::testDateAdded_data() {
    QTest::addColumn<QDate>("candidateDateAdded");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("A default constructed date is invalid") << QDate{} << false;
    QTest::addRow("A defined date is be valid") << QDate{2025, 4, 22} << true;
}

void TestMedium::testDateAdded() {
    QFETCH(QDate, candidateDateAdded);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<ConcreteMedium, QDate>(
        &Medium::dateAddedValidator,
        [](Medium& m) -> ValidatedField<QDate>& { return m.dateAdded(); },
        [](const Medium& m) -> const ValidatedField<QDate>& { return m.dateAdded(); },
        candidateDateAdded, shouldBeValid);
}

void TestMedium::testTitle_data() {
    QTest::addColumn<QString>("candidateTitle");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty strings are invalid titles") << QString{} << false;
    QTest::addRow("Space and CR-only strings are invalid titles") << QString{"   \n  \n"} << false;
    QTest::addRow("Every non-empty string is a valid title") << QString{"non-empty"} << true;
}

void TestMedium::testTitle() {
    QFETCH(QString, candidateTitle);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<ConcreteMedium, QString>(
        &Medium::titleValidator, [](Medium& m) -> ValidatedField<QString>& { return m.title(); },
        [](const Medium& m) -> const ValidatedField<QString>& { return m.title(); }, candidateTitle,
        shouldBeValid);
}

void TestMedium::testAuthors_data() {
    QTest::addColumn<std::set<QString>>("candidateAuthors");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty strings are invalid authors")
        << std::set{QString{}, QString{"valid"}} << false;
    QTest::addRow("Space and CR-only strings are invalid authors")
        << std::set{QString{"   \n  \n"}, QString{"valid"}} << false;
    QTest::addRow("Every non-empty string is a valid author")
        << std::set{QString{"non-empty"}} << true;
}

void TestMedium::testAuthors() {
    QFETCH(std::set<QString>, candidateAuthors);
    QFETCH(bool, shouldBeValid);

    TestValidatedSet::testValidatedFieldHelper<ConcreteMedium, QString>(
        &Medium::authorValidator, [](Medium& m) -> ValidatedSet<QString>& { return m.authors(); },
        [](const Medium& m) -> const ValidatedSet<QString>& { return m.authors(); },
        candidateAuthors, shouldBeValid);
}

void TestMedium::testLanguage_data() {
    QTest::addColumn<QString>("candidateLanguage");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty strings are invalid languages") << QString{} << false;
    QTest::addRow("Space and CR-only strings are invalid languages") << QString{"  \n "} << false;
    QTest::addRow("Every non-empty string is a valid language") << QString{"non-empty"} << true;
}

void TestMedium::testLanguage() {
    QFETCH(QString, candidateLanguage);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<ConcreteMedium, QString>(
        &Medium::languageValidator,
        [](Medium& m) -> ValidatedField<QString>& { return m.language(); },
        [](const Medium& m) -> const ValidatedField<QString>& { return m.language(); },
        candidateLanguage, shouldBeValid);
}