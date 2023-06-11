#pragma once

#include <CoreX/stub.hpp>
#include <CoreX/type_traits.hpp>
#include <CoreX/utility.hpp>

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
        return (is_same_v<T, Ts> || ... ||
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

    constexpr Variant() noexcept {
        activeVariant = 0;
        new ((FirstType*)data) FirstType();
    }

    template <typename Arg>
        requires(FindTemplateParamIndex<remove_cv_t<remove_reference_t<Arg>>,
                                        T,
                                        Ts...>() != static_cast<size_t>(-1))
    constexpr Variant(Arg&& arg) noexcept {
        using NoCVRefArg = remove_cv_t<remove_reference_t<Arg>>;
        activeVariant    = FindTemplateParamIndex<NoCVRefArg, T, Ts...>();
        new ((NoCVRefArg*)data) NoCVRefArg(Forward<Arg>(arg));
    }

    ~Variant() noexcept {
        destructorHelper<0, T, Ts...>(activeVariant);
    }

    explicit Variant(const Variant<T, Ts...>& v) noexcept {
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
        Assert(holdsAlternative<Arg>());
        return *((Arg*)data);
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
            // I know what I am doing, OR NOT?
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
            new ((Arg*)lhs.data) Arg(*((Arg*)rhs.data));
#pragma GCC diagnostic pop
        } else {
            copyHelper<currVariant + 1, Args...>(selectedVariant, lhs, rhs);
        }
    }
    template <size_t>
    void copyHelper(int, Variant&, const Variant&) noexcept {
    }

  private:
    enum : size_t {
        size = Max(sizeof(T), sizeof(Ts)...)
    };

    alignas(T) alignas(Ts...) char data[size];
    int activeVariant{-1};
};
