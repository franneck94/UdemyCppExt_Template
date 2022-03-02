#pragma once

#include <array>
#include <cmath>
#include <cstdint>

#include "AdConstants.hpp"

enum class LaneAssociationType
{
    LEFT,
    RIGHT,
    CENTER,
    NONE,
};

struct VehicleType
{
    std::int32_t id;
    LaneAssociationType lane;
    float speed_mps;
    float distance_m;
};

struct NeighborVehiclesType
{
    std::array<VehicleType, NUM_VEHICLES_ON_LANE> vehicles_left_lane;
    std::array<VehicleType, NUM_VEHICLES_ON_LANE> vehicles_center_lane;
    std::array<VehicleType, NUM_VEHICLES_ON_LANE> vehicles_right_lane;
};

struct Polynomial3rdDegreeType
{
    float a;
    float b;
    float c;
    float d;

    /**
     * @brief To compute p(x) = a^3 * x + b^2 * x + c * x + d
     */
    float operator()(const float x) const
    {
        const auto x_3 = std::powf(a, 3.0F) * x;
        const auto x_2 = std::powf(b, 2.0F) * x;
        const auto x_1 = std::powf(c, 1.0F) * x;
        const auto x_0 = d;
        return x_3 + x_2 + x_1 + x_0;
    }
};

struct LaneType
{
    Polynomial3rdDegreeType left_polynomial;
    Polynomial3rdDegreeType right_polynomial;

    float get_lateral_position(const float x) const
    {
        if (x <= 0.0F)
        {
            return (left_polynomial.d + right_polynomial.d) / 2.0F;
        }
        else
        {
            const auto left_border_distance_m = left_polynomial(x);
            const auto right_border_distance_m = right_polynomial(x);

            return (left_border_distance_m + right_border_distance_m) / 2.0F;
        }
    }
};

struct LanesType
{
    LaneType left_lane;
    LaneType center_lane;
    LaneType right_lane;
};
