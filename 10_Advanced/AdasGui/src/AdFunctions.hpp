#pragma once

#include <cstdint>

#include "AdTypes.hpp"

float kph_to_mps(const float kph);

float mps_to_kph(const float mps);

void compute_future_distance(VehicleType &vehicle, const float ego_driven_distance, const float seconds);

void compute_future_state(const VehicleType &ego_vehicle, NeighborVehiclesType &vehicles, const float seconds);

LaneAssociationType longitudinal_control(const NeighborVehiclesType &vehicles, VehicleType &ego_vehicle);


LaneAssociationType get_lane_change_request(const VehicleType &ego_vehicle,
                                            const float front_distance_abs,
                                            const float rear_distance_abs,
                                            const bool front_vehicle_too_close_collision,
                                            const bool rear_vehicle_too_close_collision);

bool gap_is_valid(const VehicleType &front_vehicle, const VehicleType &rear_vehicle, const VehicleType &ego_vehicle);

bool lateral_control(const NeighborVehiclesType &vehicles,
                     const LaneAssociationType lane_change_request,
                     VehicleType &ego_vehicle);
