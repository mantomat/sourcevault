#include "TestMedium.h"

#include "TestValidatedField.h"
#include "TestValidatedSet.h"
#include "testutils/mocks/MockConcreteMedium.h"

#include <QTest>

using Core::Model::Medium;

const auto concreteMediumBuilder{[] { return MockConcreteMedium{}; }};

void TestMedium::testCreateData() {
    QTest::addColumn<bool>("shouldBeValid");
    QTest::addColumn<QString>("candidateTitle");
    QTest::addColumn<QUuid>("candidateId");
    QTest::addColumn<QDate>("candidateDateAdded");

    QTest::addRow("An invalid title invalidates creation")
        << false << QString{} << QUuid::createUuid() << QDate::currentDate();
    constexpr QUuid invalidId;
    QTest::addRow("An invalid id invalidates creation")
        << false << QString{"valid"} << invalidId << QDate::currentDate();
    constexpr QDate invalidDate;
    QTest::addRow("An invalid dateAdded invalidates creation")
        << false << QString{"valid"} << QUuid::createUuid() << invalidDate;
    QTest::addRow("Params must all be valid for a valid creation")
        << true << QString{"valid"} << QUuid::createUuid() << QDate::currentDate();
}

void TestMedium::testCreateValidator_data() {
    testCreateData();
}

void TestMedium::testCreateValidator() {
    QFETCH(bool, shouldBeValid);
    QFETCH(QString, candidateTitle);
    QFETCH(QUuid, candidateId);
    QFETCH(QDate, candidateDateAdded);

    QCOMPARE(MockConcreteMedium::createValidator(candidateTitle, candidateId, candidateDateAdded),
             shouldBeValid);
}

void TestMedium::testId() {
    const MockConcreteMedium medium;
    const QUuid id = medium.id();

    QVERIFY2(!id.isNull(), "Ids should never be null");
    QCOMPARE(id.version(), QUuid::Version::Random);
    QCOMPARE(id.variant(), QUuid::Variant::DCE);
}

void TestMedium::testUserData() {
    MockConcreteMedium medium;

    const QString notes{"Valid notes"};
    const std::set topics{QString{"Computer science"}, QString{"Philosophy"}};
    constexpr bool favorite{true};
    constexpr auto priority{Core::Model::MediumUserData::PriorityLevel::mid};

    medium.userData().notes().set(notes);
    medium.userData().topics().set(topics);
    medium.userData().favorite() = favorite;
    medium.userData().priority().set(priority);

    const MockConcreteMedium constMedium{medium};
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
    QTest::addRow("A defined date is valid") << QDate{2025, 4, 22} << true;
}

void TestMedium::testDateAdded() {
    QFETCH(QDate, candidateDateAdded);
    QFETCH(bool, shouldBeValid);

    QCOMPARE(Medium::dateAddedValidator(candidateDateAdded), shouldBeValid);

    const MockConcreteMedium m{"valid title", QUuid::createUuid(), candidateDateAdded};
    QCOMPARE(m.dateAdded(), candidateDateAdded);
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

    QCOMPARE(Medium::titleValidator(candidateTitle), shouldBeValid);

    const QString initialTitle{"valid title"};
    MockConcreteMedium medium{initialTitle};
    QCOMPARE(medium.setTitle(candidateTitle), shouldBeValid);

    const MockConcreteMedium cmedium{std::move(medium)};
    QCOMPARE(cmedium.title(), shouldBeValid ? candidateTitle : initialTitle);
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

    TestValidatedSet::testValidatedFieldHelper<MockConcreteMedium, QString>(
        concreteMediumBuilder, &Medium::authorValidator,
        [](Medium &m) -> ValidatedSet<QString> & { return m.authors(); },
        [](const Medium &m) -> const ValidatedSet<QString> & { return m.authors(); },
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

    TestValidatedField::testValidatedFieldHelper<MockConcreteMedium, QString>(
        concreteMediumBuilder, &Medium::languageValidator,
        [](Medium &m) -> ValidatedField<QString> & { return m.language(); },
        [](const Medium &m) -> const ValidatedField<QString> & { return m.language(); },
        candidateLanguage, shouldBeValid);
}