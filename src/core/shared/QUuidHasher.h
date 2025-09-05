#ifndef QUUIDHASHER_H
#define QUUIDHASHER_H

#include <QUuid>

/**
 * Implementation of the std::hash function for QUuid, needed to use it as key in unordered_map and
 * unordered_set
 */
template <> struct std::hash<QUuid> {
    auto operator()(const QUuid &uuid) const -> std::size_t {
        return qHash(uuid);
    }
};

#endif