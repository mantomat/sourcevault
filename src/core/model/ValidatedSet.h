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
    explicit ValidatedSet(Validator newValidator)
        : validator{std::move(newValidator)} {}

    /**
     * @brief Compares the two sets. Validator equality is ignored.
     */
    auto operator==(const ValidatedSet& other) const -> bool {
        return storedElements == other.storedElements;
    }

    [[nodiscard]] auto has() const -> bool {
        return !storedElements.empty();
    }

    [[nodiscard]] auto get() const -> std::optional<std::set<T>> {
        return has() ? std::make_optional(storedElements) : std::nullopt;
    }

    /**
     * @brief Sets the current set. If the parameter contains at least an invalid element, it unsets
     * the set.
     * @return True if the parameter contains all valid elements. False otherwise.
     */
    auto set(std::set<T> newElements) -> bool {
        if (newElements.empty() ||
            std::ranges::any_of(newElements, [&](const T& item) { return !validator(item); })) {
            unset();
            return false;
        }
        storedElements = std::move(newElements);
        return true;
    }

    /**
     * @brief Adds an element to the set.
     * @return True if the element is valid and was absent. False otherwise.
     */
    auto add(T newElement) -> bool {
        if (!validator(newElement)) {
            return false;
        }
        return storedElements.insert(std::move(newElement)).second;
    }

    auto remove(const T& elementToRemove) -> bool {
        return storedElements.erase(elementToRemove);
    }

    void unset() {
        storedElements.clear();
    }
};

}

#endif
