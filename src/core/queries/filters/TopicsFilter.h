#ifndef TOPICSFILTER_H
#define TOPICSFILTER_H

#include "Filter.h"

#include <unordered_set>

namespace Core::Queries::Filters {

class TopicsFilter : public Filter {

    std::unordered_set<QString> topicsToMatch;

  public:
    ~TopicsFilter() override = default;

    explicit TopicsFilter(std::unordered_set<QString> newTopics);

    TopicsFilter(const TopicsFilter &) = default;
    TopicsFilter(TopicsFilter &&) = default;
    auto operator=(const TopicsFilter &) -> TopicsFilter & = default;
    auto operator=(TopicsFilter &&) -> TopicsFilter & = default;

    [[nodiscard]] auto clone() const -> std::unique_ptr<Filter> override;
    [[nodiscard]] auto matches(const Medium *medium) const -> bool override;
};

}

#endif