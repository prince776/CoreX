#pragma once

#include "CoreX/type_traits.hpp"
#include <CoreX/utility.hpp>
#include <cassert>

template <typename... Ts>
class Variant;

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
class Variant<T, Ts...> {
  public:
    using FirstType = variant_alternative_t<0, T, Ts...>;

    constexpr Variant() noexcept {
        activeVariant = 0;
        new ((FirstType*)data) FirstType();
    }

    template <typename Arg>
    constexpr Variant(Arg&& arg) noexcept {
        activeVariant = FindTemplateParamIndex<Arg, T, Ts...>();
        new ((Arg*)data) Arg(forward<Arg>(arg));
    }

    template <typename Arg>
    [[nodiscard]] constexpr Arg& get() noexcept {
        assert(holdsAlternative<Arg>());

        constexpr size_t selectedVariant =
            FindTemplateParamIndex<Arg, T, Ts...>();
        assert(selectedVariant == activeVariant);

        using ActiveType = variant_alternative_t<selectedVariant, T, Ts...>;
        return *((ActiveType*)(data));
    }

    template <typename Arg>
    [[nodiscard]] constexpr bool holdsAlternative() noexcept {
        if (activeVariant == InvalidVariant) {
            return false;
        }

        constexpr size_t selectedVariant =
            FindTemplateParamIndex<Arg, T, Ts...>();

        return static_cast<int>(selectedVariant) == activeVariant;
    }

    static constexpr int InvalidVariant = -1;

  private:
    enum : size_t {
        size = max(sizeof(T), sizeof(Ts)...)
    };

    alignas(T) alignas(Ts...) char data[size];
    int activeVariant{-1};
};
