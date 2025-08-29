#ifndef STRINGSCOMPARATOR_H
#define STRINGSCOMPARATOR_H

#include <QString>

namespace Tools {
class FieldScoreCalculatorRepl;
}

namespace Core::Queries::Search {

struct ScoreCalculatorConfigs {
    static constexpr double defaultContiguity{0.6};
    double contiguityBias{defaultContiguity};

    static constexpr double defaultLengthPenaltyAggressiveness{0.4};
    double lengthPenaltyAggressiveness{defaultLengthPenaltyAggressiveness};

    static constexpr size_t defaultMaxPrefixLengthForBonus{4};
    size_t maxPrefixLengthForBonus{defaultMaxPrefixLengthForBonus};
    static constexpr double defaultPrefixBonusFactor{0.2};
    double prefixBonusFactor{defaultPrefixBonusFactor};
};

class FieldScoreCalculator {

    /* Tool for live experimentation and parameter tweaking */
    friend class Tools::FieldScoreCalculatorRepl;

    QString searchTerm;
    ScoreCalculatorConfigs configs;

  public:
    struct LcsResult {
        size_t lcsLength;
        size_t maxContiguousRun;
    };

    struct IntermediateScoreResults {
        size_t lcsLength;
        size_t maxContiguousRun;
        double lcsScore;
        double contiguityScore;
        double baseScore;
        double lengthRatio;
        double lengthPenalty;
        double scoreBeforePrefixBonus;
        size_t maxCommonPrefix;
        double prefixFactor;
        double bonusStrength;
    };

    explicit FieldScoreCalculator(QString newSearchTerm, ScoreCalculatorConfigs newConfigs);

    [[nodiscard]] auto getWeightedScore(const QString &field, double fieldWeightLevel) const
        -> double;

  private:
    /**
     * @brief Returns a score in [0, 1] based on the Longest Common Subsequence between
     * `searchTerm` and `field`.
     */
    [[nodiscard]] auto computeRawScore(const QString &field) const -> double;
    [[nodiscard]] auto computeRawScore(const QString &field,
                                       IntermediateScoreResults *intermediateResultsOutput) const
        -> double;

    /**
     * @brief Calculates the length of the Longest Common Substring and the length of the longest
     * contiguous run inside it.
     */
    [[nodiscard]] static auto lcsWithContiguityBonus(const QString &field,
                                                     const QString &searchTerm) -> LcsResult;

    [[nodiscard]] static auto maxCommonPrefixLength(const QString &field, const QString &searchTerm)
        -> size_t;
};

}

#endif