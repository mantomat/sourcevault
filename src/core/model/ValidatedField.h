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
    explicit ValidatedField(Validator newValidator)
        : validator(std::move(newValidator)) {}

    /**
     * @brief Compares the data. Validator equality is ignored.
     */
    auto operator==(const ValidatedField& other) const -> bool {
        return data == other.data;
    }

    [[nodiscard]] auto has() const -> bool {
        return data.has_value();
    }

    [[nodiscard]] auto get() const -> std::optional<T> {
        return data;
    }

    /**
     * @brief Sets the field. If the parameter is invalid (i.e. the `validator` returns false), the
     * field gets unset.
     *
     * @return True if the parameter is valid and gets set. False otherwise.
     */
    auto set(T newValue) -> bool {
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
