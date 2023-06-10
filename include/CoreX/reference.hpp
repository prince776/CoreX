#pragma once

template <typename T>
class Ref {
  public:
    constexpr Ref(T& t) noexcept : t(&t) {
    }

    constexpr operator T&() const noexcept {
        return get();
    }

    constexpr T& get() const noexcept {
        assert(t != nullptr);
        return *t;
    }

    [[nodiscard]] constexpr Ref(const Ref& other) noexcept            = default;
    [[nodiscard]] constexpr Ref& operator=(const Ref& other) noexcept = default;

  private:
    T* t;
};
