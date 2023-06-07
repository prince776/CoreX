#pragma once

#include "CoreX/type_traits.hpp"
#include <CoreX/utility.hpp>
#include <cassert>

template <typename... Ts>
class TypeSequence {};

template <size_t N, typename T, typename... Ts>
struct variant_alternative : variant_alternative<N - 1, Ts...> {};

template <typename T, typename... Ts>
struct variant_alternative<0, T, Ts...> {
    using type = T;
};

template <size_t N, typename... Ts>
using variant_alternative_t = typename variant_alternative<N, Ts...>::type;

template <typename T, typename... Ts>
constexpr size_t FindTemplateParamIndex() noexcept {
    constexpr bool matches[] = {is_same_v<T, Ts>...};
    for (size_t i = 0; i < sizeof...(Ts); i++) {
        if (matches[i]) {
            return i;
        }
    }
    return npos;
}

template <typename T, typename... Ts>
constexpr bool ContainsDuplicateTemplateParam() noexcept {
    if constexpr (sizeof...(Ts) > 0) {
        return (std::is_same_v<T, Ts> || ... ||
                ContainsDuplicateTemplateParam<Ts...>());
    } else {
        return false;
    }
}

template <typename T, typename... Ts>
concept no_ambigious_type = !ContainsDuplicateTemplateParam<T, Ts...>();

template <typename T, typename... Ts>
    requires(no_ambigious_type<T, Ts...>)
class Variant {
  public:
    using FirstType = variant_alternative_t<0, T, Ts...>;

    explicit constexpr Variant() noexcept {
        activeVariant = 0;
        new ((FirstType*)data) FirstType();
    }

    template <typename Arg>
        requires(FindTemplateParamIndex<Arg, T, Ts...>() !=
                 static_cast<size_t>(-1))
    constexpr Variant(const Arg& arg) noexcept {
        activeVariant = FindTemplateParamIndex<Arg, T, Ts...>();
        new ((Arg*)data) Arg(arg);
    }

    ~Variant() noexcept {
        destructorHelper<0, T, Ts...>(activeVariant);
    }

    Variant(const Variant<T, Ts...>& v) noexcept {
        destructorHelper<0, T, Ts...>(activeVariant);
        activeVariant = v.activeVariant;
        // Deep copy by calling the required copy constructor.
        copyHelper<0, T, Ts...>(activeVariant, *this, v);
    }

    Variant& operator=(const Variant<T, Ts...>& v) noexcept {
        destructorHelper<0, T, Ts...>(activeVariant);
        activeVariant = v.activeVariant;

        // Deep copy by calling the required copy constructor.
        copyHelper<0, T, Ts...>(activeVariant, *this, v);
        return *this;
    }

    template <typename Arg>
    [[nodiscard]] constexpr Arg& get() noexcept {
        assert(holdsAlternative<Arg>());

        constexpr size_t selectedVariant =
            FindTemplateParamIndex<Arg, T, Ts...>();
        assert(selectedVariant == activeVariant);

        using ActiveType = variant_alternative_t<selectedVariant, T, Ts...>;
        return *((ActiveType*)data);
    }

    template <typename Arg>
    [[nodiscard]] constexpr bool holdsAlternative() const noexcept {
        if (activeVariant == InvalidVariant) {
            return false;
        }

        constexpr size_t selectedVariant =
            FindTemplateParamIndex<Arg, T, Ts...>();

        return static_cast<int>(selectedVariant) == activeVariant;
    }

    static constexpr int InvalidVariant = -1;

  private:
    template <size_t currVariant, typename Arg, typename... Args>
    void destructorHelper(int selectedVariant) noexcept {
        if (selectedVariant == currVariant) {
            return ((Arg*)(data))->~Arg();
        } else {
            destructorHelper<currVariant + 1, Args...>(selectedVariant);
        }
    }
    template <size_t>
    void destructorHelper(int) noexcept {
    }

    template <size_t currVariant, typename Arg, typename... Args>
    void
    copyHelper(int selectedVariant, Variant& lhs, const Variant& rhs) noexcept {
        if (selectedVariant == currVariant) {
            new ((Arg*)lhs.data) Arg(*((Arg*)rhs.data));
        } else {
            copyHelper<currVariant + 1, Args...>(selectedVariant, lhs, rhs);
        }
    }
    template <size_t>
    void copyHelper(int, Variant&, const Variant&) noexcept {
    }

  private:
    enum : size_t {
        size = max(sizeof(T), sizeof(Ts)...)
    };

    alignas(T) alignas(Ts...) char data[size];
    int activeVariant{-1};
};
