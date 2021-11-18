#pragma once

#include <cstdint>
#include <cstring>

constexpr std::int32_t INVALID_VEHICLE_ID = -1;
constexpr std::int32_t EGO_VEHICLE_ID = -2;

constexpr std::size_t NUM_VEHICLES_ON_LANE = 2;

constexpr float MAX_VIEW_RANGE_M = 100.0F;

constexpr float MAX_VEHICLE_SPEED_MPS = 130.0F / 3.6F;
constexpr float SPEED_ADAPTATION_FACTOR = 0.01F;
