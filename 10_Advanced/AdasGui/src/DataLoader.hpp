#pragma once

#include <string_view>

#include "json.hpp"

#include "AdTypes.hpp"
#include "DataLoaderConstants.hpp"
#include "DataLoaderTypes.hpp"

using json = nlohmann::json;

void init_vehicles(std::string_view filepath, NeighborVehiclesType &vehicles);

void init_ego_vehicle(std::string_view filepath, VehicleType &ego_vehicle);

void load_cycle(const std::uint32_t cycle, NeighborVehiclesType &vehicles);
