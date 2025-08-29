#include "TestFieldScoreCalculator.h"

#include "queries/search/FieldScoreCalculator.h"
#include "queries/search/SearchScoreVisitor.h"

#include <QTest>

using Core::Queries::Search::FieldScoreCalculator;
using Core::Queries::Search::FieldWeightLevels;
using Core::Queries::Search::ScoreCalculatorConfigs;

void TestFieldScoreCalculator::testGetWeightedScore_data() {
    QTest::addColumn<ScoreCalculatorConfigs>("calculatorConfigs");
    QTest::addColumn<QString>("searchTerm");
    QTest::addColumn<QString>("field");
    QTest::addColumn<double>("fieldWeight");
    QTest::addColumn<double>("expectedMinWeightMultiplier");
    QTest::addColumn<double>("expectedMaxWeightMultiplier");

    ScoreCalculatorConfigs defaultConfigs{};
    double defaultWeight{FieldWeightLevels{}.identifiers};

    QTest::addRow("Zero score (no match)")
        << defaultConfigs << "abc" << "def" << defaultWeight << 0.0 << 0.0;
    QTest::addRow("Zero score (empty search term)")
        << defaultConfigs << "" << "def" << defaultWeight << 0.0 << 0.0;
    QTest::addRow("Zero score (empty field)")
        << defaultConfigs << "abc" << "" << defaultWeight << 0.0 << 0.0;
    QTest::addRow("Zero score (both empty)")
        << defaultConfigs << "" << "" << defaultWeight << 0.0 << 0.0;

    QTest::addRow("Perfect match")
        << defaultConfigs << "abc" << "abc" << defaultWeight << 1.0 << 1.0;
    QTest::addRow("Perfect match (case insensitive)")
        << defaultConfigs << "AbC" << "aBc" << defaultWeight << 1.0 << 1.0;

    /* All the other cases return difficult-to-manually-calculate values and depend on configs.
     * For manual testing and to get a sense of how these values work, we provide a tool in the
     * `tools` directory. */
}
void TestFieldScoreCalculator::testGetWeightedScore() {
    QFETCH(ScoreCalculatorConfigs, calculatorConfigs);
    QFETCH(QString, searchTerm);
    QFETCH(QString, field);
    QFETCH(double, fieldWeight);
    QFETCH(double, expectedMinWeightMultiplier);
    QFETCH(double, expectedMaxWeightMultiplier);

    const FieldScoreCalculator calculator{std::move(searchTerm), calculatorConfigs};
    const double result{calculator.getWeightedScore(field, fieldWeight)};

    const double expectedMin{fieldWeight * expectedMinWeightMultiplier};
    const double expectedMax{fieldWeight * expectedMaxWeightMultiplier};

    QVERIFY2(result >= expectedMin && result <= expectedMax,
             qPrintable(QString("Score %1 was not in the expected range [%2, %3]")
                            .arg(result)
                            .arg(expectedMin)
                            .arg(expectedMax)));
}