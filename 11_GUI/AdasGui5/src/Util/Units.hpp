#pragma once

#include <cmath>
#include <type_traits>

template <typename T>
constexpr T PI = T(3.14159265358979323846L);

template <typename T, typename U>
[[nodiscard]] constexpr U ms_to_s(const T ms)
{
    static_assert(std::conjunction_v<std::is_integral<T>, std::is_floating_point<U>>, "Invalid types.");

    return ms / static_cast<U>(1000.0);
}

template <typename T>
[[nodiscard]] constexpr T kph_to_mps(const T kph)
{
    static_assert(std::is_floating_point_v<T>, "Must be floating point type.");

    return kph / static_cast<T>(3.6);
}

template <typename T>
[[nodiscard]] constexpr T mps_to_kph(const T mps)
{
    static_assert(std::is_floating_point_v<T>, "Must be floating point type.");

    return mps * static_cast<T>(3.6);
}
