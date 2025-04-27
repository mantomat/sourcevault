#ifndef OPTIONALFIELD_H
#define OPTIONALFIELD_H

#include <optional>

namespace Core::Model {

template <typename T> class OptionalField final {
  public:
    bool has() const {
        return data.has_value();
    }

    std::optional<T> get() const {
        return data;
    }

    template <typename U>
        requires std::convertible_to<U, T>
    void set(U&& newValue) {
        data = std::forward<U>(newValue);
    }

    void unset() {
        data.reset();
    }

  private:
    std::optional<T> data;
};

}

#endif
