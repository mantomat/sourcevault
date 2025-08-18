#ifndef CLONABLE_H
#define CLONABLE_H

#include <memory>

namespace Core::Shared {

template <typename T> class Cloneable {
  public:
    virtual ~Cloneable() = default;
    Cloneable() = default;
    Cloneable(const Cloneable &) = default;
    Cloneable(Cloneable &&) noexcept = default;
    auto operator=(const Cloneable &) -> Cloneable & = default;
    auto operator=(Cloneable &&) noexcept -> Cloneable & = default;

    [[nodiscard]] auto operator==(const Cloneable &) const -> bool = default;

    /**
     * @brief Clone this object.
     *
     * Note that covariant types support is lost due to smart pointers.
     */
    [[nodiscard]] virtual auto clone() const -> std::unique_ptr<T> = 0;
};
}

#endif