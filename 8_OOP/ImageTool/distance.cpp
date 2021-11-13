#include "distance.hpp"

double get_distance(const std::uint32_t x1, const std::uint32_t y1, const std::uint32_t x2, const std::uint32_t y2)
{
    auto a_diff = static_cast<std::int32_t>(x1) - static_cast<std::int32_t>(x2);
    auto a_squared = std::pow(a_diff, 2.0);
    auto b_diff = static_cast<std::int32_t>(y1) - static_cast<std::int32_t>(y2);
    auto b_squared = std::pow(b_diff, 2.0);
    auto distance = std::sqrt(a_squared + b_squared);

    return distance;
}
