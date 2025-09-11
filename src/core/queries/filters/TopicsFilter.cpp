#include "TopicsFilter.h"

#include <algorithm>
#include <unordered_set>
#include <utility>

namespace Core::Queries::Filters {

TopicsFilter::TopicsFilter(std::unordered_set<QString> newTopics)
    : topicsToMatch{std::move(newTopics)} {}

auto TopicsFilter::clone() const -> std::unique_ptr<Filter> {
    return std::make_unique<TopicsFilter>(topicsToMatch);
}

auto TopicsFilter::matches(const Medium *medium) const -> bool {
    if (medium == nullptr || !medium->userData().topics().has()) {
        return false;
    }
    if (topicsToMatch.empty()) {
        return true;
    }
    return std::ranges::any_of(medium->userData().topics().get().value(),
                               [this](const auto &topic) { return topicsToMatch.contains(topic); });
}

}