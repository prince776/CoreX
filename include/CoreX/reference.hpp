#pragma once

template <typename T>
class Ref {
  public:
    Ref(T& t) noexcept : t(t) {
    }

    constexpr operator T&() const noexcept {
        return get();
    }

    constexpr T& get() const noexcept {
        return t;
    }

  private:
    T& t;
};
