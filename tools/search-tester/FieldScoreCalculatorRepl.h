#ifndef SEARCH_SCORE_VISITOR_LIVE_TESTER_H
#define SEARCH_SCORE_VISITOR_LIVE_TESTER_H

#include "queries/search/FieldScoreCalculator.h"

#include <iostream>

using Core::Queries::Search::FieldScoreCalculator;
using Core::Queries::Search::ScoreCalculatorConfigs;
using IntermediateScoreResults =
    Core::Queries::Search::FieldScoreCalculator::IntermediateScoreResults;

namespace Tools {

class FieldScoreCalculatorRepl {
    FieldScoreCalculator calculator;

  public:
    FieldScoreCalculatorRepl(QString searchTerm, const ScoreCalculatorConfigs &configs)
        : calculator{std::move(searchTerm), configs} {}

    void runAndPrintDetails(const QString &field) {
        const auto res{std::make_unique<IntermediateScoreResults>()};
        const double totalScore{calculator.computeRawScore(field, res.get())};

        std::cout << "\n--- Score Details for field: \"" << field.toStdString() << "\" ---" << '\n';
        std::cout << "LCS Length:            " << res->lcsLength << '\n';
        std::cout << "Max Contiguous Run:    " << res->maxContiguousRun << '\n';
        std::cout << "LCS Score:             " << res->lcsScore << '\n';
        std::cout << "Contiguity Score:      " << res->contiguityScore << '\n';
        std::cout << "Base Score:            " << res->baseScore << '\n';
        std::cout << "Length Ratio:          " << res->lengthRatio << '\n';
        std::cout << "Length Penalty:        " << res->lengthPenalty << '\n';
        std::cout << "Score Before Bonus:    " << res->scoreBeforePrefixBonus << '\n';
        std::cout << "Max Common Prefix:     " << res->maxCommonPrefix << '\n';
        std::cout << "Prefix Factor:         " << res->maxCommonPrefix << '\n';
        std::cout << "Bonus Strength:        " << res->bonusStrength << '\n';
        std::cout << "Total Score:           " << totalScore << '\n';
        std::cout << "-------------------------------------------\n" << '\n';
    }
};

}
#endif