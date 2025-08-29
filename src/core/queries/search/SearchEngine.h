#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "FieldScoreCalculator.h"
#include "model/Medium.h"
#include "queries/search/SearchScoreVisitor.h"

#include <QString>

using Core::Model::Medium;

namespace Core::Queries::Search {

struct SearchEngineOptions {
    static constexpr size_t defaultMaxResults{20};
    static constexpr double defaultScoreThreshold{30.0};

    size_t maxResults{defaultMaxResults};
    double scoreThreshold{defaultScoreThreshold};
};

class SearchEngine {

    SearchEngineOptions options;
    std::unique_ptr<SearchScoreVisitor> visitor;

  public:
    ~SearchEngine() = default;

    SearchEngine(QString newSearchTerm,
                 SearchEngineOptions newSearchEngineOptions = SearchEngineOptions{},
                 SearchOptions newSearchOptions = SearchOptions{},
                 FieldWeightLevels newWeights = FieldWeightLevels{},
                 ScoreCalculatorConfigs newScoreConfigs = ScoreCalculatorConfigs{});

    SearchEngine(const SearchEngine &other);
    SearchEngine(SearchEngine &&) = default;

    auto operator=(const SearchEngine &other) -> SearchEngine &;
    auto operator=(SearchEngine &&) -> SearchEngine & = default;

    [[nodiscard]] auto search(const std::vector<const Medium *> &media) const
        -> std::vector<const Medium *>;

  private:
    void swap(SearchEngine &other) noexcept;
};

}

#endif
