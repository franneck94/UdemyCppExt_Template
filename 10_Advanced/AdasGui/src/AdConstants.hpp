#pragma once

#include <cstdint>
#include <cstring>

constexpr auto INVALID_VEHICLE_ID{-1};
constexpr auto EGO_VEHICLE_ID{-2};

constexpr auto NUM_LANES = std::size_t{3};
constexpr auto NUM_VEHICLES_ON_LANE = std::size_t{2};

constexpr auto LEFT_LEFT_BORDER{-4.5F};
constexpr auto LEFT_RIGHT_BORDER{-1.5F};
constexpr auto CENTER_LEFT_BORDER{-1.5F};
constexpr auto CENTER_RIGHT_BORDER{1.5F};
constexpr auto RIGHT_LEFT_BORDER{1.5F};
constexpr auto RIGHT_RIGHT_BORDER{4.5F};

constexpr auto MAX_VIEW_RANGE_M{100.0F};
constexpr auto MAX_VEHICLE_SPEED_MPS{130.0F / 3.6F};
constexpr auto SPEED_ADAPTATION_FACTOR{0.01F};
