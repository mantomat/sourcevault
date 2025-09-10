#ifndef QUERY_H
#define QUERY_H

#include "model/Medium.h"

using Core::Model::Medium;

namespace Core::Queries {

class Query {

  public:
    virtual ~Query() = default;
    Query() = default;
    Query(const Query &) = default;
    Query(Query &&) = default;
    auto operator=(const Query &) -> Query & = default;
    auto operator=(Query &&) -> Query & = default;

    [[nodiscard]] virtual auto query(std::vector<const Medium *>) const
        -> std::vector<const Medium *> = 0;
};

}

#endif