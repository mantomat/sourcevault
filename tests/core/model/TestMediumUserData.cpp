#include "TestMediumUserData.h"

#include "model/Medium.h"
#include "model/MediumUserData.h"

#include <QTest>

using Core::Model::MediumUserData;

void TestMediumUserData::testFavorite() {
    MediumUserData userData{};
    userData.favorite() = true;
    const MediumUserData constUserDataFavorite{userData};
    QCOMPARE(constUserDataFavorite.favorite(), true);

    userData.favorite() = false;
    const MediumUserData constUserDataNoFavorite{userData};
    QCOMPARE(constUserDataNoFavorite.favorite(), false);
}

void TestMediumUserData::testTopicValidator_data() {
    QTest::addColumn<QString>("candidate");
    QTest::addColumn<bool>("expected");

    QTest::addRow("An empty string should be invalid") << QString{} << false;
    QTest::addRow("Every non-empty string should be valid") << QString{"non-empty"} << true;
}
void TestMediumUserData::testTopicValidator() {
    QFETCH(QString, candidate);
    QFETCH(bool, expected);
    QCOMPARE(MediumUserData::topicValidator(candidate), expected);
}
void TestMediumUserData::testTopics() {
    MediumUserData userData{};

    const std::set validTopics{QString{"Computer science"}, QString{"Philosophy"}};
    userData.topics().set(validTopics);
    const MediumUserData constUserDataValidTopics{userData};
    QCOMPARE(constUserDataValidTopics.topics().get(), std::make_optional(validTopics));

    const std::set invalidTopics{QString{}};
    userData.topics().set(invalidTopics);
    const MediumUserData constUserDataInvalidTopics{userData};
    QCOMPARE(constUserDataInvalidTopics.topics().get(), std::nullopt);
}

void TestMediumUserData::testNotesValidator_data() {
    QTest::addColumn<QString>("candidate");
    QTest::addColumn<bool>("expected");

    QTest::addRow("An empty string should be invalid") << QString{} << false;
    QTest::addRow("Every non-empty string should be valid") << QString{"non-empty"} << true;
}
void TestMediumUserData::testNotesValidator() {
    QFETCH(QString, candidate);
    QFETCH(bool, expected);
    QCOMPARE(MediumUserData::notesValidator(candidate), expected);
}
void TestMediumUserData::testNotes() {
    MediumUserData userData{};

    const QString validNotes{"This is great!"};
    userData.notes().set(validNotes);
    const MediumUserData constUserDataValidNotes{userData};
    QCOMPARE(constUserDataValidNotes.notes().get(), std::make_optional(validNotes));

    const QString invalidNotes{};
    userData.notes().set(invalidNotes);
    const MediumUserData constUserDataInvalidNotes{userData};
    QCOMPARE(constUserDataInvalidNotes.notes().get(), std::nullopt);
}

void TestMediumUserData::testPriority() {
    MediumUserData userData{};

    constexpr auto validPriority{MediumUserData::PriorityLevel::mid};
    userData.priority().set(validPriority);
    const MediumUserData constUserDataValidPriority{userData};
    QCOMPARE(constUserDataValidPriority.priority().get(), std::make_optional(validPriority));

    userData.notes().unset();
    const MediumUserData constUserDataNoPriority{userData};
    QCOMPARE(constUserDataNoPriority.notes().get(), std::nullopt);
}