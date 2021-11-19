#pragma once

#include "DataLoader.hpp"

void render_cycle(std::size_t cycle, const VehicleType &ego_vehicle, const NeighborVehiclesType &vehicles);

void plot_lanes(const VehicleType &ego_vehicle, const NeighborVehiclesType &vehicles);

void plot_vehicle_in_table(const VehicleType &vehicle, std::string_view position_name);

void plot_table(const VehicleType &ego_vehicle, const NeighborVehiclesType &vehicles);

void plot_cycle_number(const std::uint32_t cycle);
