#include <CoreX/type_traits.hpp>

template <typename T, typename U>
concept same_as = is_same_v<T, U>;
