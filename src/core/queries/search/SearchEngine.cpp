#include "SearchEngine.h"

#include "queries/search/SearchScoreVisitor.h"

#include <algorithm>
#include <ranges>

namespace Core::Queries::Search {

SearchEngine::SearchEngine(QString newSearchTerm, SearchEngineOptions newSearchEngineOptions,
                           SearchOptions newSearchOptions, FieldWeightLevels newWeights,
                           ScoreCalculatorConfigs newScoreConfigs)
    : options{newSearchEngineOptions}
    , visitor{std::make_unique<SearchScoreVisitor>(
          std::move(newSearchTerm), std::move(newSearchOptions), newWeights, newScoreConfigs)} {}

void SearchEngine::swap(SearchEngine &other) noexcept {
    std::swap(visitor, other.visitor);
    std::swap(options, other.options);
}

SearchEngine::SearchEngine(const SearchEngine &other)
    : options{other.options}
    , visitor{std::make_unique<SearchScoreVisitor>(*other.visitor)} {}
auto SearchEngine::operator=(const SearchEngine &other) -> SearchEngine & {
    if (&other != this) {
        SearchEngine temp{other};
        swap(temp);
    }
    return *this;
}

auto SearchEngine::search(const std::vector<const Medium *> &media) const
    -> std::vector<const Medium *> {
    std::vector<std::pair<double, const Medium *>> scoredMedia;
    scoredMedia.reserve(media.size());

    for (const auto *medium : media) {
        if (medium == nullptr) {
            continue;
        }

        medium->accept(*visitor);
        double mediumScore = visitor->getScore();

        if (mediumScore >= options.scoreThreshold) {
            scoredMedia.emplace_back(mediumScore, medium);
        }
    }

    std::ranges::sort(scoredMedia, std::ranges::greater{},
                      &std::pair<double, const Medium *>::first);

    auto undecoratedView = scoredMedia | std::views::take(options.maxResults) | std::views::values;
    return std::vector<const Medium *>{undecoratedView.begin(), undecoratedView.end()};
}
}