#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "SearchScoreVisitor.h"
#include "model/Medium.h"

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
                 SearchWeights newWeights = SearchWeights{},
                 SearchOptions newOptions = SearchOptions{});

    SearchEngine(const SearchEngine &);
    SearchEngine(SearchEngine &&) = default;

    auto operator=(const SearchEngine &) -> SearchEngine &;
    auto operator=(SearchEngine &&) -> SearchEngine & = default;

    [[nodiscard]] auto search(const std::vector<const Medium *> &media) const
        -> std::vector<const Medium *>;

  private:
    void swap(SearchEngine &other) noexcept;
};

}

#endif