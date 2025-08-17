#ifndef VALIDATEDFIELD_H
#define VALIDATEDFIELD_H

#include <functional>
#include <optional>

namespace Core::Model {

template <typename T> class ValidatedField final {

    using Validator = std::function<bool(const T&)>;

    std::optional<T> data;
    Validator validator;

  public:
    /**
     * @brief Compares the data, validator equality is ignored.
     */
    auto operator==(const ValidatedField& other) const -> bool {
        return data == other.data;
    }

    explicit ValidatedField(Validator newValidator)
        : validator(std::move(newValidator)) {}
    bool has() const {
        return data.has_value();
    }

    std::optional<T> get() const {
        return data;
    }

    bool set(T newValue) {
        if (!validator(newValue)) {
            unset();
            return false;
        }
        data = std::move(newValue);
        return true;
    }

    void unset() {
        data.reset();
    }
};

}

#endif
