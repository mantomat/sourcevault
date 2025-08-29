#ifndef TESTFIELDSCORECALCULATOR_H
#define TESTFIELDSCORECALCULATOR_H

#include <QObject>

class TestFieldScoreCalculator : public QObject {
    Q_OBJECT

  private slots:
    static void testGetWeightedScore_data();
    static void testGetWeightedScore();
};

#endif