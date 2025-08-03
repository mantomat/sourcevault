#include "TestMediumUserData.h"

#include "TestValidatedField.h"
#include "TestValidatedSet.h"
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

void TestMediumUserData::testTopics_data() {
    QTest::addColumn<std::set<QString>>("candidateTopics");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty strings are invalid topics") << std::set<QString>{"", "valid"} << false;
    QTest::addRow("Empty strings are invalid topics")
        << std::set<QString>{QString{}, "valid"} << false;
    QTest::addRow("Space and CR-only strings are invalid topics")
        << std::set<QString>{"  \n ", "valid"} << false;

    QTest::addRow("Every non-empty string is a valid topic")
        << std::set<QString>{"valid1", "valid2"} << true;
}

void TestMediumUserData::testTopics() {
    QFETCH(std::set<QString>, candidateTopics);
    QFETCH(bool, shouldBeValid);

    TestValidatedSet::testValidatedFieldHelper<MediumUserData, QString>(
        &MediumUserData::topicValidator,
        [](MediumUserData& mud) -> ValidatedSet<QString>& { return mud.topics(); },
        [](const MediumUserData& mud) -> const ValidatedSet<QString>& { return mud.topics(); },
        candidateTopics, shouldBeValid);
}

void TestMediumUserData::testNotes_data() {
    QTest::addColumn<QString>("candidateNotes");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty strings are invalid notes") << QString{} << false;
    QTest::addRow("Space and CR-only strings are invalid notes") << QString{"  \n "} << false;
    QTest::addRow("Every non-empty string is a valid note") << QString{"non-empty"} << true;
}

void TestMediumUserData::testNotes() {
    QFETCH(QString, candidateNotes);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<MediumUserData, QString>(
        &MediumUserData::notesValidator,
        [](MediumUserData& mud) -> ValidatedField<QString>& { return mud.notes(); },
        [](const MediumUserData& mud) -> const ValidatedField<QString>& { return mud.notes(); },
        candidateNotes, shouldBeValid);
}

void TestMediumUserData::testPriority_data() {
    QTest::addColumn<MediumUserData::PriorityLevel>("candidatePriority");
    QTest::addColumn<bool>("shouldBeValid");

    MediumUserData::PriorityLevel unset;
    QTest::addRow("An unset priority level is invalid") << unset << false;
    QTest::addRow("Every actual priority level is valid")
        << MediumUserData::PriorityLevel::max << true;
    QTest::addRow("Every actual priority level is valid")
        << MediumUserData::PriorityLevel::min << true;
}

void TestMediumUserData::testPriority() {
    QFETCH(MediumUserData::PriorityLevel, candidatePriority);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<MediumUserData, MediumUserData::PriorityLevel>(
        &MediumUserData::priorityValidator,
        [](MediumUserData& mud) -> ValidatedField<MediumUserData::PriorityLevel>& {
            return mud.priority();
        },
        [](const MediumUserData& mud) -> const ValidatedField<MediumUserData::PriorityLevel>& {
            return mud.priority();
        },
        candidatePriority, shouldBeValid);
}