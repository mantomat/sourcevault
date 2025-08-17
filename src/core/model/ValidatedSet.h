#ifndef VALIDATEDSET_H
#define VALIDATEDSET_H

#include <algorithm>
#include <optional>
#include <set>

namespace Core::Model {

template <typename T> class ValidatedSet {
    using Validator = std::function<bool(const T&)>;

    std::set<T> storedElements;
    Validator validator;

  public:
    /**
     * @brief Compares the two sets, ignoring validator equality.
     */
    auto operator==(const ValidatedSet<T>& other) const -> bool {
        return storedElements == other.storedElements;
    }

    explicit ValidatedSet(Validator newValidator)
        : validator{std::move(newValidator)} {}

    bool set(std::set<T> newElements) {
        if (newElements.empty() ||
            std::ranges::any_of(newElements, [&](const T& item) { return !validator(item); })) {
            unset();
            return false;
        }
        storedElements = std::move(newElements);
        return true;
    }

    bool add(T newElement) {
        if (!validator(newElement))
            return false;
        return storedElements.insert(std::move(newElement)).second;
    }

    void remove(const T& elementToRemove) {
        storedElements.erase(elementToRemove);
    }

    void unset() {
        storedElements.clear();
    }

    bool has() const {
        return !storedElements.empty();
    }

    std::optional<std::set<T>> get() const {
        return has() ? std::make_optional(storedElements) : std::nullopt;
    }
};

}

#endif
