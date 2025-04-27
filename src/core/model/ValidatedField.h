#ifndef VALIDATEDFIELD_H
#define VALIDATEDFIELD_H

#include <functional>

namespace Core::Model {

template <typename T> class ValidatedField final {

public:
    using Validator = std::function<bool(const T&)>;

    explicit ValidatedField(Validator validator)
        : validator(std::move(validator)) {}

    bool has() const {
        return data.has_value();
    }

    std::optional<T> get() const {
        return data;
    }

    template <typename U>
        requires std::convertible_to<U, T>
    bool set(U&& newValue) {
        auto convertedNewValue = static_cast<T>(std::forward<U>(newValue));
        if (!validator(newValue)) {
            unset();
            return false;
        }
        data = std::move(convertedNewValue);
        return true;
    }

    void unset() {
        data.reset();
    }

private:
    std::optional<T> data;
    Validator validator;
};

}

#endif
