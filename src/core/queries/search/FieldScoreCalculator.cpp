#include "FieldScoreCalculator.h"

namespace Core::Queries::Search {

FieldScoreCalculator::FieldScoreCalculator(QString newSearchTerm, ScoreCalculatorConfigs newConfigs)
    : searchTerm{std::move(newSearchTerm)}
    , configs{newConfigs} {}

auto FieldScoreCalculator::getWeightedScore(const QString &field, double fieldWeightLevel) const
    -> double {
    if (searchTerm.isEmpty() || field.isEmpty()) {
        return 0.0;
    }
    return computeRawScore(field) * fieldWeightLevel;
}

auto FieldScoreCalculator::computeRawScore(
    const QString &field, IntermediateScoreResults *intermediateResultsOutput) const -> double {

    const QString lowerField{field.toLower()};
    const QString lowerSearchTerm{searchTerm.toLower()};
    const auto [lcsLength,
                maxContiguousRun]{lcsWithContiguityBonus(field.toLower(), searchTerm.toLower())};

    if (lcsLength == 0 || searchTerm.isEmpty() || field.isEmpty()) {
        if (intermediateResultsOutput != nullptr) {
            *intermediateResultsOutput = {};
        }
        return 0.0;
    }

    const double lcsScore{static_cast<double>(lcsLength) /
                          static_cast<double>(searchTerm.length())};
    const double contiguityScore{static_cast<double>(maxContiguousRun) /
                                 static_cast<double>(searchTerm.length())};

    const double baseScore{(lcsScore * (1 - configs.contiguityBias)) +
                           (contiguityScore * configs.contiguityBias)};

    const double lengthRatio{static_cast<double>(field.length()) / static_cast<double>(lcsLength)};
    const double lengthPenalty{baseScore * std::tanh(std::log1p(lengthRatio - 1)) *
                               configs.lengthPenaltyAggressiveness};

    const double scoreBeforePrefixBonus{baseScore - lengthPenalty};

    const size_t maxCommonPrefix{maxCommonPrefixLength(lowerField, lowerSearchTerm)};
    const double prefixFactor{
        static_cast<double>(std::min(maxCommonPrefix, configs.maxPrefixLengthForBonus)) /
        static_cast<double>(configs.maxPrefixLengthForBonus)};
    const double bonusStrength{prefixFactor * configs.prefixBonusFactor};

    // https://en.wikipedia.org/wiki/Linear_interpolation
    const double finalScore{(scoreBeforePrefixBonus * (1.0 - bonusStrength)) +
                            (1.0 * bonusStrength)};

    if (intermediateResultsOutput != nullptr) {
        *intermediateResultsOutput =
            IntermediateScoreResults{.lcsLength = lcsLength,
                                     .maxContiguousRun = maxContiguousRun,
                                     .lcsScore = lcsScore,
                                     .contiguityScore = contiguityScore,
                                     .baseScore = baseScore,
                                     .lengthRatio = lengthRatio,
                                     .lengthPenalty = lengthPenalty,
                                     .scoreBeforePrefixBonus = scoreBeforePrefixBonus,
                                     .maxCommonPrefix = maxCommonPrefix,
                                     .prefixFactor = prefixFactor,
                                     .bonusStrength = bonusStrength};
    }

    return finalScore;
}

auto FieldScoreCalculator::computeRawScore(const QString &field) const -> double {
    return computeRawScore(field, nullptr);
}

auto FieldScoreCalculator::lcsWithContiguityBonus(const QString &field, const QString &searchTerm)
    -> LcsResult {
    const auto rows{static_cast<size_t>(field.length() + 1)};
    const auto cols{static_cast<size_t>(searchTerm.length() + 1)};

    std::vector<size_t> previousLcsRow(cols, 0);
    std::vector<size_t> currentLcsRow(cols, 0);

    std::vector<size_t> previousRunRow(cols, 0);
    std::vector<size_t> currentRunRow(cols, 0);

    size_t maxContintiguousRun{0};

    for (size_t i = 1; i < rows; ++i) {
        for (size_t j = 1; j < cols; ++j) {
            if (field[static_cast<qsizetype>(i - 1)] == searchTerm[static_cast<qsizetype>(j - 1)]) {
                currentLcsRow[j] = 1 + previousLcsRow[j - 1];

                currentRunRow[j] = 1 + previousRunRow[j - 1];
                maxContintiguousRun = std::max(maxContintiguousRun, currentRunRow[j]);
            } else {
                currentLcsRow[j] = std::max(currentLcsRow[j - 1], previousLcsRow[j]);
                currentRunRow[j] = 0;
            }
        }
        std::swap(previousLcsRow, currentLcsRow);
        std::swap(previousRunRow, currentRunRow);
    }

    return LcsResult{.lcsLength = previousLcsRow[cols - 1],
                     .maxContiguousRun = maxContintiguousRun};
}

auto FieldScoreCalculator::maxCommonPrefixLength(const QString &field, const QString &searchTerm)
    -> size_t {
    size_t prefixLength = 0;
    while (prefixLength < static_cast<size_t>(field.length()) &&
           prefixLength < static_cast<size_t>(searchTerm.length()) &&
           field[static_cast<qsizetype>(prefixLength)] ==
               searchTerm[static_cast<qsizetype>(prefixLength)]) {
        prefixLength++;
    }
    return prefixLength;
}

}