#ifndef VALIDATEDSET_H
#define VALIDATEDSET_H

#include <set>

namespace Core::Model {

template <typename T> class ValidatedSet {

  public:
    using Validator = std::function<bool(const T&)>;

    explicit ValidatedSet(Validator newValidator)
        : validator{std::move(newValidator)} {}

    template <typename U>
        requires std::convertible_to<U, std::set<T>>
    bool set(U&& newElements) {
        std::set<T> elementsToSet{std::forward<U>(newElements)};
        if (elementsToSet.empty() || std::any_of(elementsToSet.begin(), elementsToSet.end(),
                                                 [&](const T& item) { return !validator(item); })) {
            unset();
            return false;
        }
        storedElements = std::move(elementsToSet);
        return true;
    }

    template <typename U>
        requires std::convertible_to<U, T>
    bool add(U&& newElement) {
        T elementToAdd{static_cast<T>(std::forward<U>(newElement))};
        if (!validator(elementToAdd))
            return false;
        return storedElements.insert(std::move(elementToAdd)).second;
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

  private:
    std::set<T> storedElements;
    Validator validator;
};

}

#endif
