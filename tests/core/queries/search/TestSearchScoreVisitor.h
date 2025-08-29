#ifndef TESTSEARCHSCOREVISITOR_H
#define TESTSEARCHSCOREVISITOR_H

#include "queries/search/FieldScoreCalculator.h"
#include "queries/search/SearchScoreVisitor.h"

#include <QObject>

using Core::Queries::Search::FieldWeightLevels;
using Core::Queries::Search::ScoreCalculatorConfigs;
using Core::Queries::Search::SearchOptions;
using Core::Queries::Search::SearchScoreVisitor;

class TestSearchScoreVisitor : public QObject {
    Q_OBJECT

    static constexpr auto visitorGenerator{[](QString searchTerm) {
        return SearchScoreVisitor{std::move(searchTerm), SearchOptions{}, FieldWeightLevels{},
                                  ScoreCalculatorConfigs{}};
    }};

  private slots:
    static void testGetScore_data();
    static void testGetScore();

    static void testVisitBook_data();
    static void testVisitBook();

    static void testVisitArticle_data();
    static void testVisitArticle();

    static void testVisitVideo_data();
    static void testVisitVideo();
};

#endif