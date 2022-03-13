#pragma once

#include <cstdint>
#include <cstring>

#include "Units.hpp"

constexpr std::int32_t EGO_VEHICLE_ID = -1;
constexpr std::int32_t NONE_VEHICLE_ID = -2;
constexpr std::size_t MAX_NUM_VEHICLES = 6;
constexpr float LONGITUDINAL_DIFFERENCE_PERCENTAGE = 0.05F;
constexpr std::size_t NUM_LANES = 3;

constexpr float EGO_VEHICLE_WIDTH_M = 2.5F;
constexpr float EGO_VEHICLE_HEIGHT_M = 5.0F;

constexpr auto CYCLE_TIME_MS = std::int64_t{50};
constexpr auto CYCLE_TIME_S = ms_to_s<std::int64_t, float>(CYCLE_TIME_MS);
